
#include "IGPS.h"


void ATData_GPS::cleanUp()
{
    memset(m_date, 0x00, ATDEV_GPS_DATE_SIZE + 1);
    memset(m_time, 0x00, ATDEV_GPS_TIME_SIZE + 1);

    m_latPos        ^= m_latPos;
    m_longPos       ^= m_longPos;

    m_latitude      = 0;
    m_longitude     = 0;
    m_altitude      = 0;
    m_speed         = 0;
}

void ATData_GPS::convertNMEALatitude(char *lat, char pos)
{
    uint16_t    deg;
    double      min;
    char        buffer[3];

    // init
    memset(buffer, 0x00, 3);

    // export degre
    strncpy(buffer, lat, 2);
    deg = atoi(buffer);

    // export min
    min = atof(lat+2);

    m_latitude = deg + min / 60;

    // position
    m_latPos = pos;

    if (pos == ATDEV_GPS_LATITUDE_S) {
        m_latitude *= -1;
    }
}

void ATData_GPS::convertNMEALongitude(char *lat, char pos)
{
    uint16_t    deg;
    double      min;
    char        buffer[4];

    // init
    memset(buffer, 0x00, 4);

    // export degre
    strncpy(buffer, lat, 3);
    deg = atoi(buffer);

    // export min
    min = atof(lat+3);

    m_longitude = deg + min / 60;

    // position
    m_longPos = pos;

    if (pos == ATDEV_GPS_LONGITUDE_W) {
        m_longitude *= -1;
    }
}

// vim: set sts=4 sw=4 ts=4 et:
