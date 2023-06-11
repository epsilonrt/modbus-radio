/*
    ModbusRadio.h - Header for Modbus Radio Library
    Copyright (C) 2023 Pascal JEAN, aka epsilonrt
*/
#include <Arduino.h>
#include <Modbus.h>
#include <RHGenericDriver.h>

#pragma once

/**
   @class ModbusRadio
   @brief Modbus over-the-air for devices supported by the RadioHead library
*/
class ModbusRadio : public Modbus {
  public:
    /**
       @brief Constructor
    */
    ModbusRadio (byte slaveId) :  m_slaveId (slaveId), m_radio (nullptr) {}

    /**
       @brief Configure ModbusRadio object

       @warning the \c init() function of the radio passed must be called **BEFORE** calling this function.

       @param radio reference to the radio object
    */
    void config (RHGenericDriver &radio);
    void config (RHGenericDriver *radio);

    /**
       @brief Task that performs all operations on MODBUS

       Call once inside loop(), all magic here !
    */
    void task();

    /**
       @brief Change the value of slave identifier
       @param slaveId identifier 1-247
    */
    inline void setSlaveId (byte slaveId) {
      m_slaveId = slaveId;
    }

    /**
       @brief Return slave identifier
    */
    inline byte getSlaveId() const {
      return m_slaveId;
    }

    /**
       @brief Broadcast address
    */
    static const byte BroadcastAddress = 0;

  protected:
    bool receiveADU ();
    bool sendPDU ();
    bool sendADU ();
    void reportServerId();

  private:
    byte  m_slaveId;
    RHGenericDriver *m_radio;

  private:
    word calcCrc (byte address, byte *pduframe, byte pdulen);
};
