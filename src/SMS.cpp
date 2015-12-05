
#include<SMS.h>


SMS_Data::cleanUp()
{
    memset(m_number, 0, SIM5218_NUM_SIZE + 1);
    memset(m_message, 0, SIM5218_SMS_SIZE + 1);
}

# vim: set sts=4 sw=4 ts=4 et:
