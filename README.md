# Modbus Radio Library for Arduino
Over-the-air implementation for RadioHead compatible modules.

[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/epsilonrt/modbus-radio?include_prereleases)](https://github.com/epsilonrt/modbus-radio/releases) 
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/epsilonrt/library/modbus-radio.svg)](https://registry.platformio.org/libraries/epsilonrt/modbus-radio) 
[![Arduino Registry](https://www.ardu-badge.com/badge/Modbus-Radio.svg)](https://www.arduinolibraries.info/libraries/modbus-radio)

[![Framework](https://img.shields.io/badge/Framework-Arduino-blue)](https://www.arduino.cc/)
[![Mega](https://github.com/epsilonrt/modbus-radio/actions/workflows/build_mega.yml/badge.svg)](https://github.com/epsilonrt/modbus-radio/actions/workflows/build_mega.yml)
[![Vidor](https://github.com/epsilonrt/modbus-radio/actions/workflows/build_vidor.yml/badge.svg)](https://github.com/epsilonrt/modbus-radio/actions/workflows/build_vidor.yml) 
[![Teensy](https://github.com/epsilonrt/modbus-radio/actions/workflows/build_teensy.yml/badge.svg)](https://github.com/epsilonrt/modbus-radio/actions/workflows/build_teensy.yml)
[![Esp32](https://github.com/epsilonrt/modbus-radio/actions/workflows/build_esp32.yml/badge.svg)](https://github.com/epsilonrt/modbus-radio/actions/workflows/build_esp32.yml) 

---

This library allows your Arduino to communicate via Modbus protocol. The Modbus is a master-slave protocol used in industrial automation and can be used in other areas, such as home automation.

The Modbus generally uses serial RS-232 or RS-485 as physical layer (then called Modbus Serial) and
TCP/IP via Ethernet or WiFi (Modbus TCP). But it is also possible to associate the Modbus application protocol on any other physical layer, such as the radio.

![Modbus Stack](https://github.com/epsilonrt/modbus-arduino/raw/master/extras/modbus-stack.png)

In the current version the library allows the Arduino operate **as a slave**, supporting Modbus over-the-air for hardware modules supported by the [RadioHead](https://github.com/epsilonrt/RadioHead/tree/mikem) library.

## How to

There are five classes corresponding to six headers that may be used:

* [Modbus-Arduino](http://github.com/epsilonrt/modbus-arduino ) - Base Library  
* [Modbus-Serial](https://github.com/epsilonrt/modbus-serial) - Modbus Serial RTU Library    
* [Modbus-Ethernet](https://github.com/epsilonrt/modbus-ethernet) - Modbus TCP Library (standard Ethernet Shield)   
* [Modbus-EtherCard](https://github.com/epsilonrt/modbus-ethercard) - Modbus TCP Library (for ENC28J60 chip)  
* [Modbus-Esp8266AT](https://github.com/epsilonrt/modbus-esp8266at) - Modbus IP Library (for ESP8266 chip with AT firmware)   
* [Modbus-Radio](https://github.com/epsilonrt/modbus-radio) - Modbus Radio Library for RadioHead compatible chips (**this repository**)  

By opting for Modbus Serial, TCP or Radio you must include in your sketch the corresponding header :

    #include <ModbusRadio.h>

## Modbus-Radio

The Modbus-Radio library allows the Arduino to communicate via Modbus protocol using the [RadioHead](https://github.com/epsilonrt/RadioHead/tree/mikem) library for radio communication.
This physical layer is not defined in the Modbus standard, but it is possible to use the Modbus application protocol over any physical layer.
For do this, the Modbus-Radio library implements the physical layer using the [RHGenericDriver](http://www.airspayce.com/mikem/arduino/RadioHead/classRHGenericDriver.html) class.  
This allows the use of any hardware module whose class is derived from this class, but also the use of encryption with the [RHEncryptedDriver](http://www.airspayce.com/mikem/arduino/RadioHead/classRHEncryptedDriver.html) class and the [Crypto](http://rweather.github.io/arduinolibs/index.html) library.  
Mention may be made, for example, of the very famous [SPI](http://www.airspayce.com/mikem/arduino/RadioHead/classRHSPIDriver.html) RFM22, RFM24, RFM69 and RFM95  modules.

The MODBUS protocol defines a simple protocol data unit (**PDU**) independent of the
underlying communication layers. The mapping of MODBUS protocol on specific buses or
network can introduce some additional fields on the application data unit (**ADU**)

![Modbus ADU](https://github.com/epsilonrt/modbus-radio/raw/main/extras/images/adu-pdu.png)

Modbus-Radio uses the **same ADU as for Mobdus RTU** OSL (Over Serial Line)

![Modbus ADU](https://github.com/epsilonrt/modbus-radio/raw/main/extras/images/adu-rtu.png)

the Address field only contains the slave address. The individual slave
devices are assigned addresses in the range of 1 – 247.   
The Address 0 is reserved as the broadcast address. All slave nodes must recognise the broadcast address.  
A master addresses a slave by placing the slave address in the address field
of the message. When the slave returns its response, it places its own address in the response address field to let the master know which slave is responding.  
The ADU includes an error–checking field that is based on a Cyclical Redundancy Checking (CRC) method performed on the message contents.  
The CRC field checks the contents of the entire message. The CRC field contains a 16–bit value implemented as two 8–bit bytes.

Thus it is not necessary to use the classes derived from RHDatagram because this protocol provides addressing, integrity control and acknowledgment. 

There are six [examples](https://github.com/epsilonrt/modbus-radio/tree/master/examples) that can be accessed from the Arduino interface, once you have installed the library.

* [Lamp](https://github.com/epsilonrt/modbus-radio/blob/master/examples/Lamp/Lamp.ino): Use a coil to turn on and off a LED (0x05 and 0x01 functions)  
* [LampEncrypted](https://github.com/epsilonrt/modbus-radio/blob/master/examples/LampEncrypted/LampEncrypted.ino): Use a coil to turn on and off a LED (0x05 and 0x01 functions) with encrypted communication.  
* [LampDimmer](https://github.com/epsilonrt/modbus-radio/blob/master/examples/LampDimmer/LampDimmer.ino): Use a holding register to control the brightness of a LED (0x06 and 0x03 functions)   
* [Switch](https://github.com/epsilonrt/modbus-radio/blob/master/examples/Switch/Switch.ino): Use a discrete input to read the state of a switch (0x02 function)  
* [TempSensor](https://github.com/epsilonrt/modbus-radio/blob/master/examples/TempSensor/TempSensor.ino): Use a input register to read the temperature from a sensor (0x04 function)  
* [Servo](https://github.com/epsilonrt/modbus-radio/blob/master/examples/Servo/Servo.ino): Use a holding register to control the position of a servo motor (0x06 an 0x03 function). Show how to define boundaries for the register.  

Let's look at the example Lamp.ino (only the parts concerning Modbus and RadioHead will be commented):

    #include <ModbusRadio.h>

Inclusion of the necessary library.

    const byte SlaveId = 10;

Sets the slave address. This value is the address of the slave device. The address range is 1 to 247.

    const int Lamp1Coil = 0;

Sets the Modbus register to represent the switch. This value is the offset (0-based) to be placed in its supervisory or testing software.
Note that if your software uses offsets 1-based the set value there should be 101, for this example.

    const float frequency = 868.0;

Sets the frequency of the radio module. This value is in MHz and can be 315, 433, 868 or 915.

    RH_RF95 radio;

Create the radio instance (RH_RF95) to be used. The syntax is equal to RadioHead class, and supports the formats described in the [documentation](http://www.airspayce.com/mikem/arduino/RadioHead/classRH__RF95.html#a274c170acdb53f55fd45445ab7f51889).

    ModbusRadio mb (SlaveId);

Create the mb instance (ModbusRadio) to be used with the slave address.

    if (!radio.init()) {
      Console.println ("radio init failed !");
      Console.flush();
      exit (EXIT_FAILURE);
    }

Initializes the radio module. If the initialization fails, the program is terminated. The default configuration is used : Bw = 125 kHz, Cr = 5 (4/5), Sf = 7 (128chips/symbol), CRC on. The transmitter power is 13dBm.

If necessary, you can change the configuration by calling the following methods:

    radio.setModemConfig(RH_RF95::Bw500Cr45Sf128);

Sets the modem configuration. 

    radio.setFrequency (frequency);

Sets the frequency of the radio module. This value is in MHz and can be 315, 433, 868 or 915.

    radio.setTxPower (20);

Sets the transmitter power in dBm. The default is 13dBm and can be set between +5 and +23 dBm.

    mb.config (radio);
  
Initializes the ModbusRadio instance by passing the radio instance as parameter.

    mb.setAdditionalServerData ("LAMP");
    
Set the additional server data. This data will be sent in the response to the Report Server ID function (0x11). 
    
    mb.setDebug (Console);

Sets the debug output. The debug output is used to display the Modbus frames and other information. The debug output can be any class that implements the Print interface, such as Serial or Console.

Then we have:

    mb.addCoil (Lamp1Coil);

Adds the register type Coil (digital output) that will be responsible for activating the LED or lamp and verify their status. 

The library allows you to set an initial value for the register:

    mb.addCoil (Lamp1Coil, true);

In this case the register is added and set to true. If you use the first form the default value is false.

    mb.task ();

This method makes all magic, answering requests and changing the registers if necessary, it should be called only once, early in the loop.

    digitalWrite (LedPin, mb.coil (Lamp1Coil));

Finally the value of Lamp1Coil register is used to drive the lamp or LED.

In much the same way, the other examples show the use of other methods available in the library:

    void addCoil (offset word, bool value)
    void addHreg (offset word, word value)
    void addIsts (offset word, bool value)
    void addIreg (offset word, word value)

Adds registers and configures initial value if specified.

    bool setCoil (offset word, bool value)
    bool setHreg (offset word, word value)
    bool setIsts (offset word, bool value)
    bool setIReg (offset word, word value)

Sets a value to the register.

    bool coil (offset word)
    word hreg  (word offset)
    bool ists (offset word)
    word ireg (word offset)

Returns the value of a register.

**Notes:**

1. The RadioHead library is not included in the Modbus-Radio library, you must install it separately if you are not using [Platformio](https://platformio.org/) or [Arduino Cli](https://arduino.github.io/arduino-cli). This library is published in the [Arduino Library Manager](https://www.arduinolibraries.info/libraries/modbus-radio), so you can install it directly from the Arduino interface. It is important to know that RadioHead is published according to 2 licenses: the GNU Public License and a commercial license if you do not wish to distribute the sources of your software.  
2. If you want to use encryption, you must install the [Crypto](http://rweather.github.io/arduinolibs/index.html) library and define RH_ENABLE_ENCRYPTION_MODULE, by uncomment #define RH_ENABLE_ENCRYPTION_MODULE at the bottom of RadioHead.h, or using a -D option passed to the compiler. 


License
=======
The code in this repo is licensed under the BSD New License. See LICENSE for more info.

