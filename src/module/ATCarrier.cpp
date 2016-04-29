
#include "ATCarrier.h"


uint8_t ATCarrier::setSIMPin(uint16_t pin)
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CPIN, pin);

    return this->waitDevice(this->sendATCmd());
}

uint8_t ATCarrier::getNetworkStatus()
{
    strncpy_P(m_cmdBuffer, ATDEV_CMD_CREG, ATDEV_BUFF_CMD_SIZE);
   
    if (this->sendATCmd() == ATDEV_OK) {
        // parse answer
        this->parseInternalData();

        return atoi(this->getParseElement(2));
    }

    return ATDEV_NETSTAT_UNKNOWN;
}


// vim: set sts=4 sw=4 ts=4 et:
