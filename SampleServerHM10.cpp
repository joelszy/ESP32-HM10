/**
 * Create a new HM10 BLE server.
 * Joel Szymanski 
 * 03/01/2019
 * based on Kolban ESP32-Snippets
 * https://github.com/nkolban/esp32-snippets
 * 
 */
#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"
#include <esp_log.h>
#include <string>
#include <Task.h>
#include <System.h>
#include "sdkconfig.h"
#include "SampleServerHM10.h"


static char LOG_TAG[] = "HM10";

/**
 * Helper sleep func
 */
static inline void sleep_ms(int delay) { vTaskDelay(delay / portTICK_RATE_MS); };

/**
 * HM10 Callback : send back to client console OK msg
 */
class BLECharacteristicCallbacksHM10: public BLECharacteristicCallbacks {
public:
	~BLECharacteristicCallbacksHM10() {};
	void onRead(BLECharacteristic* pCharacteristic) {
		ESP_LOGD(LOG_TAG, "JS ! OnRead");

	};
	void onWrite(BLECharacteristic* pCharacteristic) {
		ESP_LOGD(LOG_TAG, "JS ! OnWrite [%s]", pCharacteristic->getValue().c_str());
		std::string s = 	"OK received [" + pCharacteristic->getValue() + "]";
		pCharacteristic->setValue(s);
		pCharacteristic->notify(true);
	};
};

static BLECharacteristicCallbacksHM10 cbHM10;

/**
 * HM10 Server main task
 * initialize server with HM10 services ffe0 & ffe1
 * send hello message to client console every 5sec 
 */
class MainBLEServer: public Task {
	void run(void *data) {
		ESP_LOGD(LOG_TAG, "Starting BLE work!");

		BLEDevice::init("ESP32_HM10");
		BLEServer* pServer = BLEDevice::createServer();

		BLEService* pService = pServer->createService("0000ffe0-0000-1000-8000-00805F9B34FB");

		BLECharacteristic* pCharacteristic = pService->createCharacteristic(
			BLEUUID("0000ffe1-0000-1000-8000-00805F9B34FB"),
			BLECharacteristic::PROPERTY_BROADCAST | BLECharacteristic::PROPERTY_READ  |
			BLECharacteristic::PROPERTY_NOTIFY    | BLECharacteristic::PROPERTY_WRITE |
			BLECharacteristic::PROPERTY_INDICATE
		);

		pCharacteristic->setValue("Hello World!");

		BLE2902* p2902Descriptor = new BLE2902();
		p2902Descriptor->setNotifications(true);
		pCharacteristic->addDescriptor(p2902Descriptor);

		// >> JS : add HM10 callback
		pCharacteristic->setCallbacks(&cbHM10);
		// << JS :

		pService->start();

		BLEAdvertising* pAdvertising = pServer->getAdvertising();
		pAdvertising->addServiceUUID(BLEUUID(pService->getUUID()));
		pAdvertising->start();

		ESP_LOGD(LOG_TAG, "Advertising started!");

		// >> JS : send hello messages every 5 sec
		std::string s;
		char str[128+1];
	
		for( int i=0; i<100000; i++)
		{
			snprintf(str,128,"Hello %d",i);
			pCharacteristic->setValue(std::string(str));
			pCharacteristic->notify(true);
			delay(5000);
		}
		// << JS :
		
		delay(1000000);
	}
};

/**
 * SampleServerHM10
 * Launch main server process and check memory leak every 10sec.
 */
void SampleServerHM10(void)
{
	//esp_log_level_set("*", ESP_LOG_DEBUG);
	MainBLEServer* pMainBleServer = new MainBLEServer();
	pMainBleServer->setStackSize(20000);
	pMainBleServer->start();

	// JS : check config
	ESP_LOGD(LOG_TAG, "***********************************");
	std::string s =	System::getIDFVersion();
	ESP_LOGD(LOG_TAG," IDF : %s", s.c_str());
	System::dumpPinMapping();
	ESP_LOGD(LOG_TAG, "***********************************\n\n\n");

	// JS : check memory leaks
	for (int i=0; i<10000; i++)
	{
		ESP_LOGD(LOG_TAG, "***********************************");
		System::dumpHeapInfo();
		ESP_LOGD(LOG_TAG, "***********************************\n\n\n");
		sleep_ms(15000);
	}
	// JS : 

} // SampleServerHM10



