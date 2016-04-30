
#include "ESP8266.h"

uint8_t _ESP8266_WifiClient::restartChip()
{
    // reset command
    m_timeOut = ATDEV_WIFI_TIMEOUT_RESTART;
    strncpy_P(m_cmdBuffer, ATDEV_CMD_RST, ATDEV_BUFF_CMD_SIZE);

    return this->sendATCmd();
}

uint8_t _ESP8266_WifiClient::initializeWifiClient()
{
    // set Mode to station
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CWMODE_CUR, ESP8266_OPT_MODE_STATION);

    // send command
    if (this->sendATCmd() != ATDEV_OK) {
        return ATDEV_ERR_UNEXPECTED_RESULT;
    }

    return this->waitDevice(ATDEV_OK);
}


// vim: set sts=4 sw=4 ts=4 et:
