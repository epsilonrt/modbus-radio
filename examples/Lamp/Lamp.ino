/**
  @file Lamp.ino
  Modbus-Arduino Example - Lamp (Modbus over-the-air for devices supported by the RadioHead library)

  <WARNING> You MUST connect monitor to Serial, otherwise the sketch will not start !
  
  Copyright (C) 2023 Pascal JEAN aka epsilonrt
  https://github.com/epsilonrt/modbus-radio
*/
#include <ModbusRadio.h>
#include <RH_RF95.h>

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

// RFM95 Module
// You can use any module derived from RHGenericDriver, uncomment the module corresponding to your module
RH_RF95 radio;
// RH_RF95 radio (6, 7); // LoRasSpi with Arduino MKR VIDOR 4000

// ModbusRadio object
ModbusRadio mb (SlaveId);

void setup() {
  Console.begin (115200);
  while (!Console);
  Console.print ("Modbus-Radio Lamp Example\nSlave @: ");
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

  mb.config (radio); // Set radio driver
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
