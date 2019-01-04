# ESP32-HM10
HM10 Sample Server for ESP32


# For who ?

For HM10 Users : If you are using HM10 modules for you IOT applications, you can switch easily to ESP32 module for a all in one solution - CPU + HM10 BT module -

For new IOT DIY : if you plan to send command to a device with your phone, it can be very usefull.

On IOS you will be able to send command using existing apps, for example : 

[FREE IOS HM10 Bluetooth Serial application](https://itunes.apple.com/fr/app/hm10-bluetooth-serial-lite/id1030454675?mt=8)

Nota Bene : with paid version 'PRO' you can create buttons to send commands, even complex commands.

# How to install

## First install ESP32 devt environnement

Everything available on [ESP-IDF GITHUB](https://github.com/espressif/esp-idf/blob/master/README.md). If you want to know more about ESP32 developpement environnement see [ESP-IDF Programming Guide](https://dl.espressif.com/doc/esp-idf/latest/api-guides/build-system.html).

## Then create your own application developpement environnement

1/ Create a working dir 'esp' with 'components' subdir and cd into it: 
    
    $ mkdir esp
    $ cd esp
    esp$ mkdir components
    esp$ cd components
    esp/components$
    
2/ Clone [Neil Kolban esp32-snippets](https://github.com/nkolban/esp32-snippets):
    
    esp/components$ git clone https://github.com/nkolban/esp32-snippets.git
        
3/ Clone my [ESP32-HM10 sample](https://github.com/joelszy/ESP32-HM10):

    esp/components$ git clone https://github.com/joelszy/ESP32-HM10.git

    if you type :  esp/components$ ls
    
you should see now following directories :
    
    ESP32-HM10  esp32-snippets

4/ Create 'main' dir in 'esp' :
    
    esp/components$ cd ..
    esp$ mkdir main
    esp$ cd main
    esp/main$
    
5/ Create the 'main.cpp' file containing call to main function: 

    esp/main$ nano main.cpp
    
copy paste following code and save:

    #include "sdkconfig.h"
    #include <esp_system.h>
    #include "SampleServerHM10.h"

    /**
     * Main file for running the HM10 BLE device.
     */
    extern "C" void app_main(void) {
        SampleServerHM10();
    } // app_main
    
6/ Copy the 'component.mk' file from 'esp/components/ESP32-HM10/component.mk' to 'esp/main'

    esp/main$ cp ../components/ESP32-HM10/component.mk ./
    
    
7/ Configure makefile

    esp/main$ cd ..
    esp$ 
    
Create partition file :
    
    esp$ nano partitions.csv
    
copy paste following table and save:
    
    # Name,   Type, SubType, Offset,  Size, Flags
    # Note: if you change the phy_init or app partition offset, make sure to change the offset in Kconfig.projbuild
    nvs,      data, nvs,     0x9000,   0x4000
    otadata,  data, ota,     0xd000,   0x2000
    phy_init, data, phy,     0xf000,   0x1000
    factory,  0,    0,       0x10000,  0x200000
    ota_0,    0,    ota_0,   0x210000, 0x100000
    storage,  data, spiffs,  0x310000, 0x0f0000,

Create Makefile :
    
    esp$ nano Makefile
    
copy paste following code and save:
    
    #
    # This is a project Makefile. It is assumed the directory this Makefile resides in is a
    # project subdirectory.
    #

    PROJECT_NAME := HM10_serverapp

    include $(IDF_PATH)/make/project.mk
    
    
8/ Run make

    esp$ make -j4
    
the first time you run make command you should see sdkconfig application popup.
    
mandatory :
    
    Partition Table /
        Partition Table --> Custom partition table CSV  --> needed in debug mode as app size is to big to fit in default partition size
        Custom partition CSV file --> partitions.csv

    Serial Flasher Config /
        Flash size: 4 MB --> needed with custom partition 

    Compiler Options /
        [*] Enable C++ Exception
        
    Component Config /
        [*] Bluetooth
        [*] Bluedroid Enable
            Bluetooth event (callback to application) task size --> 8192 
    
non mandatory :
    
    Component config / Log output /
    Default log verbosity --> Debug --> needed to see message through esp monitoring exchanged between ESP server & client 
    
    Serial Flasher Config /
        Default baud rate: 921600  --> faster flashing

    Component Config /
        CPU frequency --> 240MHz --> faster CPU
    
9/ Flash and monitor

    esp$ make flash
    esp$ make monitor
    
    
10/ Test it

    *open HM10 Bluetooth Serial on your IOS device.
    
    *apps should scan BLE device and show a : <ESP32_HM10> device.
    
    *click on it. Now you should receive Hello messages every 5seconds.
    
    *you can send messages "XXX".
    
    *after sending a message you will reveive a "OK received [XXX]"


## Customize for your application and enjoy :)
    
    
    
    