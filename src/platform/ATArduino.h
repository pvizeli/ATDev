
#ifndef ATARDUINO_H
#define ATARDUINO_H

// includes
#include "../core/ATDev.h"

// timeouts
#define ATDEV_FIRST_ATCMD_TIMEOUT 5000
#define ATDEV_POWER_RETRY 10

/**
 * Object for handle all communication with ATDEV chip from Arduino IDE
 *
 */
class ATArduino : public virtual ATDev
{
    public:
   
        /**
         * Initialize this Object.
         *
         * It will set the Serial object for communication and his baudrate.
         * ATDev initialize all system relevant configs.
         *
         * @param UART              Serial object to device
         * @param baudrate          Serial baudrate
         */
        void initialize(HardwareSerial *UART, long baudrate);

        /**
         * Power On and check communication to device
         *
         * Is modPin not equal to 0 it will power off and check for AT
         * communication and is the device ready for commands.
         *
         * @param onPinMod          PIN for on/off the shield or 0
         * @return                  ATDEV Okay/Error
         */
        uint8_t onPower(uint8_t onPinMod = 0);
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
