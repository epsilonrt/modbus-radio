/**
  @file LampEncrypted.ino
  Modbus-Arduino Example - Lamp (Modbus over-the-air for devices supported by the RadioHead library)

  <WARNING> You MUST connect monitor to Serial, otherwise the sketch will not start !

  Example with AES128 encryption, the RH_ENABLE_ENCRYPTION_MODULE macro must be defined.

  In order to enable this module you must uncomment #define RH_ENABLE_ENCRYPTION_MODULE at the bottom 
  of RadioHead.h, or using a -D option passed to the compiler. 
  
  But ensure you have installed the Crypto directory from arduinolibs first: http://rweather.github.io/arduinolibs/index.html

  Copyright (C) 2023 Pascal JEAN aka epsilonrt
  https://github.com/epsilonrt/modbus-radio
*/
#include <ModbusRadio.h>
#include <RH_RF95.h>
#include <RHEncryptedDriver.h>
#include <AES.h>

// Defines the serial port as the console on the Arduino platform
#define Console Serial
// Slave address (1-247)
const byte SlaveId = 10;
// Modbus Registers Offsets (0-9999)
const int Lamp1Coil = 0;
// Lamp Pin that will be controlled
const int LampPin = LED_BUILTIN;
// const int LampPin = 4; // LoRasSpi
// LoRa frequency, uncomment the frequency corresponding to your module
const float frequency = 868.0;
// const float frequency = 915.0;
// const float frequency = 433.0;

// Encryption key, the same key must be configured on all communicating elements
const uint8_t encryptKey[16] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f' };

// RFM95 Module
// You can use any module derived from RHGenericDriver, uncomment the module corresponding to your module
RH_RF95 radio;
// RH_RF95 radio (6, 7); // LoRasSpi with Arduino MKR VIDOR 4000

// Cipher AES128 (Crypto lib)
AES128 cipher;

// Driver that assembles cipher and RF transmitter
RHEncryptedDriver driver (radio, cipher);

// ModbusRadio object
ModbusRadio mb (SlaveId);

void setup() {
  Console.begin (115200);
  while (!Console);
  Console.print ("Modbus-Radio LampEncrypted Example\nSlave @: ");
  Console.println (SlaveId);

  // Set LampPin mode
  pinMode (LampPin, OUTPUT);

  // Defaults after init are 434.0MHz, 13dBm,
  // Bw = 125 kHz, Cr = 5 (4/5), Sf = 7 (128chips/symbol), CRC on
  if (!radio.init()) {
    Console.println ("radio init failed !");
    Console.flush();
    exit (EXIT_FAILURE);
  }

  // Setup ISM frequency
  radio.setFrequency (frequency);

  // You can change the modulation parameters with eg
  // radio.setModemConfig(RH_RF95::Bw500Cr45Sf128);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 2 to 20 dBm:
  // radio.setTxPower(20, false);

  // Set encryption key
  if (cipher.setKey (encryptKey, sizeof (encryptKey))) {

    Console.println ("Encryption enabled");
  }
  else {

    Console.println ("Unable to set secret key !");
    Console.flush();
    exit (EXIT_FAILURE);
  }

  mb.config (driver); // Set radio driver
  mb.setAdditionalServerData ("LAMP"); // for Report Server ID function (0x11)
  mb.setDebug (Console); // use Serial for debuging

  // Add Lamp1Coil register - Use addCoil() for digital outputs
  mb.addCoil (Lamp1Coil);
  Console.println ("Waiting for incoming messages....");
}

void loop() {
  // Call once inside loop() - all magic here
  mb.task();

  // Attach LampPin to Lamp1Coil register
  digitalWrite (LampPin, mb.coil (Lamp1Coil));
}
