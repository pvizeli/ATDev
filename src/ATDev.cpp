
#include<ATDev.h>


ATDev::ATDev()
{
    memset(m_endBuffer, 0, SIM5218_BUFF_END_SIZE + 1);
    memset(m_cmdBuffer, 0, SIM5218_BUFF_CMD_SIZE + 1);
    memset(m_msgBuffer, 0, SIM5218_BUFF_MSG_SIZE + 1);

    m_msgCount      ^= m_msgCount;
    m_endCount      ^= m_endCount;
    m_timeOutMillis ^= m_timeOutMillis;

    m_isGPS         = false;
}

ATDev::~ATDev()
{

}

uint8_t ATDev::sendATCmd(bool defaultEnd, uint8_t timeOut)
{
    // init Buffer
    memset(m_cmdBuffer, 0, SIM5218_BUFF_CMD_SIZE + 1);
    memset(m_msgBuffer, 0, SIM5218_BUFF_MSG_SIZE + 1);

    // init Counter
    m_msgCount ^= m_msgCount;
    
    // is it the default AT end or had his own end?
    if (defaultEnd) {
        // set default end of AT communication
        memset(m_endBuffer, 0, SIM5218_BUFF_END_SIZE + 1);
        strncpy_P(m_endBuffer, SIM5218_END_OK, SIM5218_BUFF_END_SIZE);    
    }
    m_endCount = strlen(m_endBuffer);

    // Clear input Serial buffer
    while (Serial.read() >= 0);

    // Send AT command
    Serial.println(m_cmdBuffer);

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
    Serial.flush();

    // process answer
    do {

        // if data in serial input buffer
        while (Serial.available()) {

            // buffer is full
            if (m_msgCount + 1 >= SIM5218_BUFF_MSG_SIZE) {
                return SIM5218_ERR_BUFFER_FULL;
            }

            // read into buffer
            m_msgBuffer[++m_msgCount] = Serial.read();
        }

        // check is it the end of AT Command in answer buffer
        if (m_msgCount >= m_endCount && 
                strstr(m_msgBuffer[m_msgCount - m_endCount]) != 0) {
            return 0; 
        }
        // Error
        else if (m_msgCount >= SIM5218_END_ERROR_SIZE &&
                strstr_P(m_msgBuffer[m_msgCount - SIM5218_END_ERROR_SIZE], SIM5218_END_ERROR) != 0) {
            return SIM5218_ERR_ERROR_RECEIVED;
        }

    } while (m_timeOutMillis <= millis()); // timeout

    return SIM5218_ERR_TIMEOUT;
}

# vim: set sts=4 sw=4 ts=4 et: