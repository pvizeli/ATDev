
#ifndef ATEASYSMS_H
#define ATEASYSMS_H

// includes
#include "ATDev.h"
#include "ISMS.h"

// commands
#define ATDEV_CMD_CPMS PSTR("AT+CPMS=\"SM\",\"SM\",\"SM\"")
#define ATDEV_CMD_CMGF PSTR("AT+CMGF=1")

// strings
#define ATDEV_STR_CMGL PSTR("+CMGL:")

/**
 * Default AT SMS command handle object
 */
class ATEasySMS : public ISMS, public virtual ATDev
{
    protected:
        
        /**
         *
         */
        uint8_t doDeleteSMS(uint8_t idx, uint8_t flag);

    public:

        /**
         *
         */
        virtual uint8_t initializeSMS();

        /**
         *
         */
        virtual uint8_t sendSMS();

        /**
         *
         */
        virtual uint8_t receiveSMS(uint8_t idx);

        /**
         *
         */
        virtual uint8_t deleteSMS(uint8_t idx) {
            return this->doDeleteSMS(idx, ATDEV_OPT_CMGD_DEL_IDX);
        }

        /**
         *
         */
        virtual uint8_t deleteAllSMS(uint8_t flag) {
            return this->doDeleteSMS(0x00, flag);
        }

        /**
         *
         */
        virtual uint8_t readNextIdxSMS();
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
