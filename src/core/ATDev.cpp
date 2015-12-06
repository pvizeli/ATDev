
#include "ATDev.h"


ATDev::ATDev()
{
    m_hwSerial      = NULL;

    memset(m_endBuffer, 0, ATDEV_BUFF_END_SIZE + 1);
    memset(m_cmdBuffer, 0, ATDEV_BUFF_CMD_SIZE + 1);
    memset(m_msgBuffer, 0, ATDEV_BUFF_MSG_SIZE + 1);

    m_msgCount      ^= m_msgCount;
    m_endCount      ^= m_endCount;
    m_timeOutMillis ^= m_timeOutMillis;
    m_onModulePin   ^= m_onModulePin;
}

uint8_t ATDev::sendATCmd(uint8_t timeOut = ATDEV_DEFAULT_TIMEOUT, bool defaultEnd = true)
{
    // UART initialize?
    if (m_hwSerial == NULL) {
        return ATDEV_ERR_INITIALIZE;
    }

    // init Buffer
    memset(m_msgBuffer, 0, ATDEV_BUFF_MSG_SIZE + 1);

    // init Counter
    m_msgCount ^= m_msgCount;
    
    // is it the default AT end or had his own end?
    if (defaultEnd) {
        // set default end of AT communication
        memset(m_endBuffer, 0, ATDEV_BUFF_END_SIZE + 1);
        strncpy_P(m_endBuffer, ATDEV_END_OK, SIM5218_BUFF_END_SIZE);    
    }
    m_endCount = strlen(m_endBuffer);

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

    // process answer
    do {

        // if data in serial input buffer
        while (m_hwSerial->available()) {

            // buffer is full
            if (m_msgCount + 1 >= SIM5218_BUFF_MSG_SIZE) {
                return ATDEV_ERR_BUFFER_FULL;
            }

            // read into buffer
            m_msgBuffer[++m_msgCount] = m_hwSerial->read();
        }

        // check is it the end of AT Command in answer buffer
        if (m_msgCount >= m_endCount && 
                strstr(m_msgBuffer[m_msgCount - m_endCount]) != 0) {
            return 0; 
        }
        // Error
        else if (m_msgCount >= ATDEV_END_ERROR_SIZE &&
                strstr_P(m_msgBuffer[m_msgCount - ATDEV_END_ERROR_SIZE], ATDEV_END_ERROR) != 0) {
            return ATDEV_ERR_ERROR_RECEIVED;
        }

    } while (m_timeOutMillis <= millis()); // timeout

    return ATDEV_ERR_TIMEOUT;
}

uint8_t ATDev::initialize(HardwareSerial *UART, uint8_t baudrate, uint8_t onPinMod)
{
    m_hwSerial      = UART;
    m_onModulePin   = onPinMod;

    pinMode(m_onModulePin, OUTPUT);
    m_hwSerial->begin(baudrate);

    return this->doWakeup(2000);
}

uint8_t ATDev::doWakeup(uint8_t timeOut = ATDEV_DEFAULT_TIMEOUT)
{
    if (this->isReady(1000) > ATDEV_OK) {

        digitalWrite(m_onModulePin, HIGH);
        delay(3000);
        digitalWrite(m_onModulePin, LOW);

        return this->isReady(timeOut);
    }

    return ATDEV_OK;
}

uint8_t ATDev::isReady(uint8_t timeOut = ATDEV_DEFAULT_TIMEOUT)
{
    memset(m_cmdBuffer, 0, ATDEV_BUFF_CMD_SIZE + 1);
    strnstr_P(m_cmdBuffer, ATDEV_CMD_AT, ATDEV_BUFF_CMD_SIZE);
   
    return this->sendATCmd(timeOut);
}

// vim: set sts=4 sw=4 ts=4 et:
