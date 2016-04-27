
#include "ATArduino.h"


void ATArduino::initialize(HardwareSerial *UART, long baudrate)
{
    // Init ATDev
    ATDev::initialize(UART);

    // init serial interface / Arduino IDE Style
    m_hwSerial->begin(baudrate);
}

uint8_t ATArduino::onPower(uint8_t onPinMod)
{
    // Set pinMode for power on/off
    if (onPinMod != 0) {
        pinMode(onPinMod, OUTPUT);
    }

    // First AT CMD timeout
    m_timeOut = ATDEV_FIRST_ATCMD_TIMEOUT;

    // Wait for starting up modul
    delay(m_timeOut);

    if (this->isReady() != ATDEV_OK) {

        // if a Pin is a button to power on/off the shield
        if (onPinMod != 0) {
            digitalWrite(onPinMod, HIGH);
            delay(3000);
            digitalWrite(onPinMod, LOW);
            delay(1000);
        }

        // check is modem response
        for (uint8_t i = 0; i < ATDEV_POWER_RETRY; i++) {
            if (this->isReady() == ATDEV_OK) {
                return this->waitDevice(ATDEV_OK);
            }
        }
    }
    // power is allready on
    else {
        return ATDEV_OK;
    }

    // timeout
    return ATDEV_ERR_TIMEOUT;
}

// vim: set sts=4 sw=4 ts=4 et:
