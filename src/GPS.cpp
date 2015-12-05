
#include<GPS.h>


GPS_Data::cleanUp()
{
    memset(m_latitude, 0, GPS_LATITUDE_SIZE + 1);
    memset(m_longitude, 0, GPS_LONGITUDE_SIZE + 1);
    memset(m_altitude, 0, GPS_ALTITUDE_SIZE + 1);
    memset(m_speed, 0, GPS_SPEED_SIZE + 1);

    m_latPos    = 0x00;
    m_longPos   = 0x00;
}

# vim: set sts=4 sw=4 ts=4 et:
