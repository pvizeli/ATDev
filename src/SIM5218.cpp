
#include<SIM5218.h>
#include<Arduino.h>

SIM5218_sms::clearUp()
{
    memset(m_number, 0, SIM5218_BUFF_NUM_SIZE + 1);
    memset(m_message, 0, SIM5218_BUFF_SMS_SIZE + 1);
}


# vim: set sts=4 sw=4 ts=4 et:
