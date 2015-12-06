
#ifndef ATEASYSMS_H
#define ATEASYSMS_H

// includes
#include "ATDev.h"
#include "ISMS.h"

// commands
#define ATDEV_CMD_CPMS PSTR("AT+CPMS=\"SM\",\"SM\",\"SM\"")
#define ATDEV_CMD_CMGS PSTR("AT+CMGS=\"%s\"")
#define ATDEV_CMD_CMGF PSTR("AT+CMGF=%d")

// option
#define ATDEV_OPT_CMGF_TXT 0x01

/**
 * Default AT SMS command handle object
 */
class ATEasySMS : public ISMS, public virtual ATDev
{

    public:

        /**
         *
         */
        uint8_t sendSMS();
}

#endif

// vim: set sts=4 sw=4 ts=4 et:
