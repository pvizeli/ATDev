
#include "ISMS.h"


void SMS_Data::cleanUp()
{
    memset(m_number, 0, ATDEV_SMS_NUM_SIZE + 1);
    memset(m_message, 0, ATDEV_SMS_TXT_SIZE + 1);
}


// vim: set sts=4 sw=4 ts=4 et:
