
#include "ISMS.h"

ATData_SMS::ATData_SMS()
{
    m_number        = NULL;
    m_message       = NULL;

    m_numberSize    ^= m_numberSize;
    m_messageSize   ^= m_messageSize;
}

ATData_SMS::ATData_SMS(char *number, uint8_t numberSize, 
                       char *message, uint16_t messageSize) :
    m_number(number),
    m_numberSize(numberSize),
    m_message(message),
    m_messageSize(messageSize)
{
    this->cleanUp();
}

void ATData_SMS::cleanUp()
{
    // not init
    if (m_number == NULL || m_message == NULL) {
        return;
    }

    // cleanup
    memset(m_number, 0x00, m_numberSize);
    memset(m_message, 0x00, m_messageSize);
}


// vim: set sts=4 sw=4 ts=4 et:
