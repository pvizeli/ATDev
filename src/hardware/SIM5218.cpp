
#include "SIM5218.h"

#ifdef SIM5218_USE_GPS

uint8_t SIM5218::onOffGPS(uint8_t onOff, uint8_t opt)
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

uint8_t SIM5218::receiveGPS()
{
    // gps not running
    if (!m_isGPSOn) {
        return ATDEV_GPS_ERR_NOT_INIT;
    }
    strncpy_P(m_cmdBuffer, ATDEV_CMD_CGPSINFO, ATDEV_BUFF_CMD_SIZE);

    // AT+CPGSINFO
    if (this->sendATCmd() != ATDEV_OK) {
        return ATDEV_ERR_UNEXPECTED_RESULT;
    }

    // parse
    if (this->parseInternalData() < 2) {
        return ATDEV_GPS_ERR_NO_GPS_DATA;
    }

    ////
    // Parse GPS Data to gpsData class

    strncpy(m_gpsData.m_latitude, this->getParseElement(1), ATDEV_GPS_LATITUDE_SIZE);
    strncpy(m_gpsData.m_longitude, this->getParseElement(3), ATDEV_GPS_LONGITUDE_SIZE);
    strncpy(m_gpsData.m_altitude, this->getParseElement(7), ATDEV_GPS_ALTITUDE_SIZE);
    strncpy(m_gpsData.m_speed, this->getParseElement(8), ATDEV_GPS_SPEED_SIZE);

    m_gpsData.m_latPos    = *(this->getParseElement(2));
    m_gpsData.m_longPos   = *(this->getParseElement(4));

    return ATDEV_OK;
}

#endif

// vim: set sts=4 sw=4 ts=4 et:

