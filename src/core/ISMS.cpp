
#include "ISMS.h"


void SMS_Data::cleanUp()
{
    memset(m_number, 0, ATDEV_SMS_NUM_SIZE + 1);
    memset(m_message, 0, ATDEV_SMS_TXT_SIZE + 1);
}

bool ISMS::isCMSError()
{
    // found "+CMS ERROR: %d"
    if (strstr_P(m_msgBuffer, ATDEV_STR_CMS) != NULL) {
        return true;
    }

    return false;
}


// vim: set sts=4 sw=4 ts=4 et:
