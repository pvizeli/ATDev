
#include "ATDev.h"


ATDev::ATDev()
{
    m_hwSerial      = NULL;

    memset(m_endBuffer, 0x00, ATDEV_BUFF_END_SIZE + 1);
    memset(m_cmdBuffer, 0x00, ATDEV_BUFF_CMD_SIZE + 1);
    memset(m_msgBuffer, 0x00, ATDEV_BUFF_MSG_SIZE + 1);

    m_msgPtr        ^= m_msgPtr;
    m_endSize       ^= m_endSize;
    m_timeOutMillis ^= m_timeOutMillis;
    m_onModulePin   ^= m_onModulePin;
}

uint8_t ATDev::sendATCmd(uint16_t timeOut, bool abruptEnd)
{
    // UART initialize?
    if (m_hwSerial == NULL) {
        return ATDEV_ERR_INITIALIZE;
    }

    // init Buffer
    memset(m_msgBuffer, 0x00, ATDEV_BUFF_MSG_SIZE);

    // init Counter
    m_msgPtr ^= m_msgPtr;
    
    // is it the default AT end or had his own end?
    if (!abruptEnd) {
        // set default end of AT communication
        memset(m_endBuffer, 0x00, ATDEV_BUFF_END_SIZE);
        strncpy_P(m_endBuffer, ATDEV_END_OK, ATDEV_BUFF_END_SIZE);    
    }
    m_endSize = strlen(m_endBuffer);

    // Clear input Serial buffer
    while (m_hwSerial->read() >= 0);

    // Send AT command
    m_hwSerial->println(m_cmdBuffer);

    // Calc Timeout
    m_timeOutMillis = millis();

    // If millis() overloaded
    if (m_timeOutMillis + timeOut < m_timeOutMillis) {
        m_timeOutMillis = timeOut - (0xFFFF - m_timeOutMillis);
    }
    else {
        m_timeOutMillis += timeOut;
    }

    // wait until all data are send
    m_hwSerial->flush();

    // clean comand buffer for next use
    memset(m_cmdBuffer, 0x00, ATDEV_BUFF_CMD_SIZE);

    // process answer
    do {

        // if data in serial input buffer
        while (m_hwSerial->available()) {

            // buffer is full
            if (m_msgPtr >= ATDEV_BUFF_MSG_SIZE) {
                return ATDEV_ERR_BUFFER_FULL;
            }

            // read into buffer
            m_msgBuffer[m_msgPtr++] = m_hwSerial->read();

            // if abrupt end of communication is set
            if (abruptEnd && m_msgPtr >= m_endSize && 
                    strstr(&m_msgBuffer[m_msgPtr - m_endSize], m_endBuffer) != 0) {
                return ATDEV_OK; 
            }
        }

        // check is it the end of AT Command in answer buffer
        if (m_msgPtr >= m_endSize && 
                strstr(m_msgBuffer, m_endBuffer) != 0) {
            return ATDEV_OK; 
        }
        // Error
        else if (m_msgPtr >= ATDEV_END_ERROR_SIZE &&
                strstr_P(m_msgBuffer, ATDEV_END_ERROR) != 0) {
            return ATDEV_ERR_ERROR_RECEIVED;
        }

    } while (m_timeOutMillis <= millis()); // timeout

    return ATDEV_ERR_TIMEOUT;
}

uint8_t ATDev::initialize(HardwareSerial *UART, uint16_t baudrate, uint8_t onPinMod)
{
    m_hwSerial      = UART;
    m_onModulePin   = onPinMod;

    pinMode(m_onModulePin, OUTPUT);
    m_hwSerial->begin(baudrate);

    return this->onPower();
}

uint8_t ATDev::onPower(uint16_t timeOut)
{
    if (this->isReady(1000) != ATDEV_OK) {

        digitalWrite(m_onModulePin, HIGH);
        delay(3000);
        digitalWrite(m_onModulePin, LOW);

        // timeout
        timeOut += millis();

        // overlfow millis()
        if (timeOut < millis()) {
            timeOut -= 0xFF - (millis());
        }

        // check is modem response
        while (timeOut > millis()) {
            if (this->isReady()) {
                return ATDEV_OK;
            }
        }
    }
    // power is allready on
    else {
        return ATDEV_OK;
    }

    // timeout
    return ATDEV_ERR_TIMEOUT;
}

uint8_t ATDev::isReady(uint16_t timeOut)
{
    strncpy_P(m_cmdBuffer, ATDEV_CMD_AT, ATDEV_BUFF_CMD_SIZE);
   
    return this->sendATCmd(timeOut);
}

// vim: set sts=4 sw=4 ts=4 et:
