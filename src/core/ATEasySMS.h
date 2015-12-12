
#ifndef ATEASYSMS_H
#define ATEASYSMS_H

// includes
#include "ATDev.h"
#include "ISMS.h"

// commands
#define ATDEV_CMD_CPMS PSTR("AT+CPMS=\"SM\",\"SM\",\"SM\"")
#define ATDEV_CMD_CMGS PSTR("AT+CMGS=\"%s\"")
#define ATDEV_CMD_CMGF PSTR("AT+CMGF=1")
#define ATDEV_CMD_CMGD PSTR("AT+CMGD=%d,%d")
#define ATDEV_CMD_CMGR PSTR("AT+CMGR=%d")
#define ATDEV_CMD_CMGL PSTR("AT+CMGL=\"%S\"")

// option
#define ATDEV_OPT_CMGD_DEL_ALL 0x04
#define ATDEV_OPT_CMGD_DEL_IDX 0x00
#define ATDEV_OPT_CMGD_DEL_READED 0x03

#define ATDEV_OPT_CMGL_UNREAD PSTR("REC UNREAD")
#define ATDEV_OPT_CMGL_READ PSTR("REC READ")
#define ATDEV_OPT_CMGL_ALL PSTR("ALL")

// Size
# ifndef ATDEV_EASYSMS_LIST_SIZE
#  define ATDEV_EASYSMS_LIST_SIZE 2
# endif

/**
 * Default AT SMS command handle object
 */
class ATEasySMS : public ISMS, public virtual ATDev
{
    protected:
        
        /** */
        uint8_t m_listNewMessageIdx[ATDEV_EASYSMS_LIST_SIZE];

        /**
         *
         */
        uint8_t doDeleteSMS(uint8_t idx, uint8_t flag);

    public:

        ATEasySMS();

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
        virtual uint8_t readAllNewSMS();
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
