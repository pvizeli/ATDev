
#ifndef ISMS_H
#define ISMS_H

// include
# include "ATDev.h"

// commands
#define ATDEV_CMD_CMGS PSTR("AT+CMGS=\"%s\"")
#define ATDEV_CMD_CMGD PSTR("AT+CMGD=%s,%d")
#define ATDEV_CMD_CMGR PSTR("AT+CMGR=%d")
#define ATDEV_CMD_CMGL PSTR("AT+CMGL=\"%S\"")

// option
#define ATDEV_OPT_CMGD_DEL_ALL 0x04
#define ATDEV_OPT_CMGD_DEL_IDX 0x00
#define ATDEV_OPT_CMGD_DEL_READED 0x03

#define ATDEV_OPT_CMGL_UNREAD PSTR("REC UNREAD")
#define ATDEV_OPT_CMGL_READ PSTR("REC READ")
#define ATDEV_OPT_CMGL_ALL PSTR("ALL")

// data size
// the real size is SIZE+1 for char buffer
# ifndef ATDEV_SMS_TXT_SIZE
#  define ATDEV_SMS_TXT_SIZE 160
# endif
# define ATDEV_SMS_NUM_SIZE 15

// option
#define ATDEV_SMS_DEL_ALL 0x04
#define ATDEV_SMS_DEL_READED 0x03

// Val
#define ATDEV_SMS_NO_MSG 0xFFFF

// Timeouts
#define ATDEV_SMS_TIMEOUT_SEND 20000
#define ATDEV_SMS_TIMEOUT_DELALL 10000
#define ATDEV_SMS_TIMEOUT_DEL 5000
#define ATDEV_SMS_TIMEOUT_LIST 5000
#define ATDEV_SMS_TIMEOUT_READ 5000

/**
 * Object for store sms data
 */
class SMS_Data
{
    public:
        SMS_Data() {
            this->cleanUp();
        }

        /** Phone number string */
        char m_number[ATDEV_SMS_NUM_SIZE + 1];

        /** SMS body string */
        char m_message[ATDEV_SMS_TXT_SIZE + 1];

        /**
         * Cleanup all buffers.
         *
         * Call this function to make this object ready for new message.
         */
        void cleanUp();
};

/**
 * Object for handle all communication with ATDEV chip
 */
class ISMS
{
    public:

        /** SMS Object for receive/send sms */
        SMS_Data m_smsData;

        /**
         * Initialize the AT device for handle SMS.
         *
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t initializeSMS() = 0;

        /**
         * Send SMS that store in @see m_smsData.
         *
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t sendSMS() = 0;

        /**
         * Receive SMS from SIM to @see m_smsData.
         *
         * @param idx               Message index SIM store number
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t receiveSMS(uint16_t idx) = 0;

        /**
         * Delete SMS from SIM storage.
         *
         * @param idx               Message index SIM store number
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t deleteSMS(uint16_t idx) = 0;

        /**
         * Delete all SMS from SIM storage.
         *
         * @param flag              ATDEV_SMS_DEL_ALL / ATDEV_SMS_DEL_READED
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t deleteAllSMS(uint8_t flag) = 0;

        /**
         * Get the next message index number from SIM storage.
         *
         * @param lastIdx           Last readed index number for next ID
         *                          or 0 if you want read from first store
         * @return                  Index nummber or ATDEV_SMS_NO_MSG
         */
        virtual uint16_t readNextIdxSMS(uint16_t lastIdx = 0) = 0;
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
