
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

    return ret;
}

uint8_t _SIM5218_GPS::receiveGPS()
{
    // gps not running
    if (!m_isGPSOn) {
        return ATDEV_ERR_GPS_INIT;
    }
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

    // Convert format from NMEA
    m_gpsData.convertNMEALatitude(this->getParseElement(1));
    m_gpsData.convertNMEALongitude(this->getParseElement(1));

    m_gpsData.m_altitude = atof(this->getParseElement(7));
    m_gpsData.m_speed = atof(this->getParseElement(8));

    strncpy(m_gpsData.m_date, this->getParseElement(5), ATDEV_GPS_DATE_SIZE);
    strncpy(m_gpsData.m_time, this->getParseElement(6), ATDEV_GPS_TIME_SIZE);

    m_gpsData.m_latPos    = *(this->getParseElement(2));
    m_gpsData.m_longPos   = *(this->getParseElement(4));

    return ATDEV_OK;
}

// vim: set sts=4 sw=4 ts=4 et:
