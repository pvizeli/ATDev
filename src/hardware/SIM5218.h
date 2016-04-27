
#ifndef SIM5218_H
#define SIM5218_H

#include "../ATDev_HW.h"

// commands
#define ATDEV_CMD_CGPS PSTR("AT+CGPS=%d,%d")
#define ATDEV_CMD_CGPSINFO PSTR("AT+CGPSINFO")

// option
#define SIM5218_OPT_GPS_ON 0x01
#define SIM5218_OPT_GPS_OFF 0x00
#define SIM5218_OPT_GPS_STD 0x01

/**
 * Object for handle GPS communication with SIM5218 chip
 */
class _SIM5218_GPS : 
    public virtual ATDev,
    public IGPS
{
    protected :
        /**
         * Activate or disable GPS handling on chip
         */
        uint8_t onOffGPS(uint8_t onOff, uint8_t opt);

    public:
        /**
         * Implement from @see IGPS
         */
        virtual uint8_t initializeGPS() {
            return ATDEV_OK;
        }

        virtual uint8_t onGPS() {
            return this->onOffGPS(SIM5218_OPT_GPS_ON, SIM5218_OPT_GPS_STD);
        }

        virtual uint8_t offGPS() {
            return this->onOffGPS(SIM5218_OPT_GPS_OFF, SIM5218_OPT_GPS_STD);
        }

        virtual uint8_t receiveGPS();
};

/**
 * Object for handle all communication with SIM5218 chip
 */
class SIM5218 : 
#ifdef SIM5218_USE_ARDUINO
    public ATArduino,
#else
    public virtual ATDev,
#endif
    public ATCarrier
#ifdef SIM5218_USE_EASYSMS
    ,public ATEasySMS
#endif
#ifdef SIM5218_USE_GPS
    ,public _SIM5218_GPS
#endif
{

};

#endif

// vim: set sts=4 sw=4 ts=4 et:
