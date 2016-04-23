
#ifndef ATCARRIER_H
#define ATCARRIER_H

// includes
#include "ATDev.h"

// commands
#define ATDEV_CMD_CPIN PSTR("AT+CPIN=%d")
#define ATDEV_CMD_CREG PSTR("AT+CREG?")

// Network status
#define ATDEV_NETSTAT_NOT_REGISTERED 0x00
#define ATDEV_NETSTAT_REGISTERED 0x01
#define ATDEV_NETSTAT_SEARCHING 0x02
#define ATDEV_NETSTAT_DENIED 0x03
#define ATDEV_NETSTAT_UNKNOWN 0x04
#define ATDEV_NETSTAT_ROAMING 0x05

/**
 * Object for handle all communication with ATDEV chip
 *
 */
class ATCarrier : public virtual ATDev
{
    public:
   
        /**
         * Unlock the SIM.
         *
         * @return                  ATDEV Okay/Error
         */
        uint8_t setSIMPin(uint16_t pin);

        /**
         * Get the network status of the device back.
         *
         * @return                  ATDEV network status or Okay/Error
         */
        uint8_t getNetworkStatus();
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
