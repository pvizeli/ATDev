
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

uint8_t ATDev::sendATCmd(uint16_t timeOut, bool abruptEnd, char* readBuf, uint16_t readBufSize)
{
    // UART initialize?
    if (m_hwSerial == NULL) {
        return ATDEV_ERR_INITIALIZE;
    }

    ////
    // init buffers
    // Set default buffer for internal handling
    if (readBuf == NULL) {
        readBuf = m_msgBuffer;
    }

    // init read Buffer
    memset(readBuf, 0x00, readBufSize);

    // init Counter
    m_msgPtr ^= m_msgPtr;
    
    // is it the default AT end or had his own end?
    // set default end of AT communication
    if (!abruptEnd) {
        memset(m_endBuffer, 0x00, ATDEV_BUFF_END_SIZE);
        strncpy_P(m_endBuffer, ATDEV_END_OK, ATDEV_BUFF_END_SIZE);    
    }
    m_endSize = strlen(m_endBuffer);

    // Clear input Serial buffer
    while (m_hwSerial->read() >= 0);

    ////
    // Send AT command
    m_hwSerial->println(m_cmdBuffer);

    // clean comand buffer for next use
    memset(m_cmdBuffer, 0x00, ATDEV_BUFF_CMD_SIZE);

    ////
    // Calc Timeout
    m_timeOutMillis = millis();

    // If millis() overloaded
    if (m_timeOutMillis + timeOut < m_timeOutMillis) {
        m_timeOutMillis = timeOut - (0xFFFF - m_timeOutMillis);
    }
    else {
        m_timeOutMillis += timeOut;
    }
    
    ////
    // wait until all data are send
    m_hwSerial->flush();

    ////
    // process answer
    do {

        // if data in serial input buffer
        while (m_hwSerial->available()) {

            // buffer is full
            if (m_msgPtr >= readBufSize) {
                return ATDEV_ERR_BUFFER_FULL;
            }

            // read into buffer
            readBuf[m_msgPtr++] = m_hwSerial->read();

            // if abrupt end of communication is set
            if (abruptEnd && m_msgPtr >= m_endSize && 
                    strstr(&readBuf[m_msgPtr - m_endSize], m_endBuffer) != 0) {
                return ATDEV_OK; 
            }
        }

        ////
        // check is it the end of AT Command in answer buffer
        if (m_msgPtr >= m_endSize && strstr(readBuf, m_endBuffer) != 0) {
            return ATDEV_OK; 
        }
        // Error
        else if (m_msgPtr >= ATDEV_END_ERROR_SIZE &&
                strstr_P(readBuf, ATDEV_END_ERROR) != 0) {
            return ATDEV_ERR_ERROR_RECEIVED;
        }

    } while (m_timeOutMillis > millis()); // timeout

    return ATDEV_ERR_TIMEOUT;
}

void ATDev::initialize(HardwareSerial *UART, long baudrate, uint8_t onPinMod)
{
    m_hwSerial      = UART;
    m_onModulePin   = onPinMod;

    // init serial interface
    pinMode(m_onModulePin, OUTPUT);
    m_hwSerial->begin(baudrate);
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
            if (this->isReady() == ATDEV_OK) {
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

uint8_t ATDev::setSIMPin(uint16_t pin, uint16_t timeOut)
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CPIN, pin);

    return this->sendATCmd(timeOut);
}

// vim: set sts=4 sw=4 ts=4 et:
