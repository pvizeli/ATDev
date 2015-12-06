
#ifndef IGPS_H
#define IGPS_H

// includes

// data size
// the real size is SIZE+1 for char buffer
#define ATDEV_GPS_LATITUDE_SIZE 15
#define ATDEV_GPS_LONGITUDE_SIZE 15
#define ATDEV_GPS_ALTITUDE_SIZE 15
#define ATDEV_GPS_SPEED_SIZE 8

// error codes
#define ATDEV_GPS_ERR_CONNECTION 0x21
#define ATDEV_GPS_ERR_NOT_INIT 0x22

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
        char m_latitude[ATDEV_GPS_LATITUDE_SIZE];
        char m_longitude[ATDEV_GPS_LONGITUDE_SIZE];
        char m_altitude[ATDEV_GPS_ALTITUDE_SIZE];
        char m_speed[ATDEV_GPS_SPEED_SIZE];

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
class IGPS
{
    IGPS() {
        m_isGPSOn = false;
    }

    ~IGPS();

    protected:

        /** is GPS module init and run */
        bool m_isGPSOn;

    public:

        /** GPS Object for store gps data */
        GPS_Data m_gpsData;

        /**
         *
         *
         */
        virtual uint8_t onGPS() = 0;

        /**
         *
         *
         */
        virtual uint8_t offGPS();

        /**
         *
         */
        virtual uint8_t receiveGPS();
}

#endif

# vim: set sts=4 sw=4 ts=4 et:
