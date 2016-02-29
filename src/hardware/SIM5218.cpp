
#include "SIM5218.h"

uint8_t _SIM5218_GPS::onOffGPS(uint8_t onOff, uint8_t opt)
{
    uint8_t ret;
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CGPS, onOff, opt);

    // send command
    ret = this->sendATCmd();

    // command success
    if (ret == ATDEV_OK) {
        m_isGPSOn = !m_isGPSOn;
    }

    return this->waitDevice(ret);
}

uint8_t _SIM5218_GPS::receiveGPS()
{
    char latPos, longPos, *date;

    // gps not running
    if (!m_isGPSOn) {
        return ATDEV_ERR_GPS_INIT;
    }

    // GPS command
    strncpy_P(m_cmdBuffer, ATDEV_CMD_CGPSINFO, ATDEV_BUFF_CMD_SIZE);

    // AT+CPGSINFO
    if (this->sendATCmd() != ATDEV_OK) {
        return ATDEV_ERR_UNEXPECTED_RESULT;
    }

    // parse
    if (this->parseInternalData() < 2) {
        return ATDEV_ERR_GPS_DATA;
    }

    ////
    // Parse GPS Data to gpsData class
    m_gpsData.cleanUp();

    // Convert format from NMEA
    latPos    = *(this->getParseElement(2));
    longPos   = *(this->getParseElement(4));

    m_gpsData.convertNMEALatitude(this->getParseElement(1), latPos);
    m_gpsData.convertNMEALongitude(this->getParseElement(3), longPos);

    m_gpsData.m_altitude = atof(this->getParseElement(7));
    m_gpsData.m_speed = atof(this->getParseElement(8));

    strncpy(m_gpsData.m_time, this->getParseElement(6), ATDEV_GPS_TIME_SIZE);

    // convert Date
    memset(m_gpsData.m_date, 0x00, ATDEV_GPS_DATE_SIZE);
    date = this->getParseElement(5);

    m_gpsData.m_date[0] = 0x32;
    m_gpsData.m_date[1] = 0x30;
    m_gpsData.m_date[2] = date[4];
    m_gpsData.m_date[3] = date[5];
    m_gpsData.m_date[4] = date[0];
    m_gpsData.m_date[5] = date[1];
    m_gpsData.m_date[6] = date[2];
    m_gpsData.m_date[7] = date[3];


    return ATDEV_OK;
}

// vim: set sts=4 sw=4 ts=4 et:
