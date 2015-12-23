
#ifndef IGPS_H
#define IGPS_H

// include
#include "ATDev.h"

// data size
// the real size is SIZE+1 for char buffer
#define ATDEV_GPS_DATE_SIZE 6
#define ATDEV_GPS_TIME_SIZE 8
#define ATDEV_GPS_SPEED_SIZE 8

/**
 * Object for store gps data
 */
class GPS_Data
{
    public:
        GPS_Data() {
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
         */
        void convertNMEALatitude(char *lat);

        /**
         * Convert the string to longitude from NMEA format.
         *
         * @param lat           Input NMEA string
         */
        void convertNMEALongitude(char *lat);
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

        /** GPS Object for store gps data */
        GPS_Data m_gpsData;

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
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t receiveGPS() = 0;
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
