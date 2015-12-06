
#ifndef SIM5218_H
#define SIM5218_H

#include "../ATDev_HW.h"

// module on pin
#define SIM5218_ONPIN 2

// commands
#define ATDEV_CMD_CGPS PSTR("AT+CGPS=%d,%d")
#define ATDEV_CMD_CGPSINFO PSTR("AT+CGPSINFO")

// option
#define ATDEV_OPT_GPS_ON 0x01
#define ATDEV_OPT_GPS_OFF 0x00
#define ATDEV_OPT_GPS_STD 0x01

/**
 * Object for handle all communication with SIM5218 chip
 */
class SIM5218 : public virtual ATDev, public IDevice, public ATSms, public IGPS
{
    public:

        /**
         * Implement from IDevice
         */
        virtual uint8_t initialize(HardwareSerial *UART, uint8_t baudrate);

        /**
         * Impl. from IGPS
         */
        virtual uint8_t onGPS();
        virtual uint8_t offGPS();
        virtual uint8_t receiveGPS();
        
}

#endif

# vim: set sts=4 sw=4 ts=4 et:
