
#ifndef IGPS_H
#define IGPS_H

// include
#include "../core/ATDev.h"

// data size
// the real size is SIZE+1 for char buffer
#define ATDEV_GPS_DATE_SIZE 8
#define ATDEV_GPS_TIME_SIZE 8

#define ATDEV_GPS_LATITUDE_N 0x4E
#define ATDEV_GPS_LATITUDE_S 0x53
#define ATDEV_GPS_LONGITUDE_W 0x57
#define ATDEV_GPS_LONGITUDE_E 0x45

/**
 * Object for store gps data
 */
class ATData_GPS
{
    public:
        ATData_GPS() {
            this->cleanUp();
        }

        /** Latitude */
        double m_latitude;

        /** Longitude */
        double m_longitude;

        /** Altidue */
        double m_altitude;

        /** Speed */
        double m_speed;

        /** Date in format YYYYMMDD */
        char m_date[ATDEV_GPS_DATE_SIZE +1];

        /** Time in format HHMMSS.S */
        char m_time[ATDEV_GPS_TIME_SIZE +1];
        
        /** N/S */
        char m_latPos;

        /** W/E */
        char m_longPos;

        /**
         * Cleanup all buffers.
         *
         * Call this function to make this object ready for new gps data.
         */
        void cleanUp();

        /**
         * Convert the string to latitude from NMEA format.
         *
         * @param lat           Input NMEA string
         * @param pos           Position 'N' or 'S'
         */
        void convertNMEALatitude(char *lat, char pos);

        /**
         * Convert the string to longitude from NMEA format.
         *
         * @param lat           Input NMEA string
         * @param pos           Position 'W' or 'E'
         */
        void convertNMEALongitude(char *lat, char pos);
};

/**
 * Object for handle all communication with AT and GPS
 */
class IGPS
{
    protected:

        /** is GPS module init and run */
        bool m_isGPSOn;

    public:

        /**
         * Cleanups
         */
        IGPS() {
            m_isGPSOn = false;
        }

        /**
         * Initiialize the GPS module.
         *
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t initializeGPS() = 0;

        /**
         * Start GPS session on device.
         *
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t onGPS() = 0;

        /**
         * Stop GPS session on device.
         *
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t offGPS() = 0;

        /**
         * Receive GPS updates from device.
         *
         * @param gps               GPS Data object
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t receiveGPS(ATData_GPS *gps) = 0;
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
