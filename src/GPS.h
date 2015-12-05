
#ifndef GPS_H
#define GPS_H

// includes
#include<ATDev.h>

// commands
#define ATDEV_CMD_CGPS PSTR("AT+CGPS=%d,%d")
#define ATDEV_CMD_CGPSINFO PSTR("AT+CGPSINFO")

// option
#define ATDEV_OPT_GPS_ON 0x01
#define ATDEV_OPT_GPS_OFF 0x00
#define ATDEV_OPT_GPS_STD 0x01

// data size
// the real size is SIZE+1 for char buffer
#define GPS_LATITUDE_SIZE 15
#define GPS_LONGITUDE_SIZE 15
#define GPS_ALTITUDE_SIZE 15
#define GPS_SPEED_SIZE 8

// error codes
#define ATDEV_ERR_GPS_CONNECTION 0x21
#define ATDEV_ERR_GPS_NOT_INIT 0x22

/**
 * Object for store gps data
 */
class GPS_Data
{
    private:
        GPS_Data() {
            this->cleanUp();
        }

    public:
        char m_latitude[GPS_LATITUDE_SIZE];
        char m_longitude[GPS_LONGITUDE_SIZE];
        char m_altitude[GPS_ALTITUDE_SIZE];
        char m_speed[GPS_SPEED_SIZE];

        char m_latPos;
        char m_longPos;

        /**
         * Cleanup all buffers.
         *
         * Call this function to make this object ready for new gps data.
         */
        void cleanUp();
}

/**
 * Object for handle all communication with AT and GPS
 */
class GPS : public virtual ATDev
{
    GPS();
    ~GPS();

    protected:

        /** is GPS module init and run */
        bool m_isGPS;

    public:

        /** GPS Object for store gps data */
        GPS_Data m_gpsData;

        /**
         *
         *
         */
        uint8_t startGPS();

        /**
         *
         *
         */
        uint8_t stopGPS();

        /**
         *
         */
        uint8_t updateGPS();
}

#endif

# vim: set sts=4 sw=4 ts=4 et:
