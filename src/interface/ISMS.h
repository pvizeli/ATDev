
#ifndef ISMS_H
#define ISMS_H

// include
# include "../core/ATDev.h"

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

// option
#define ATDEV_SMS_DEL_ALL 0x04
#define ATDEV_SMS_DEL_READED 0x03

// Val
#define ATDEV_SMS_NO_MSG 0xFFFF

// Timeouts
#define ATDEV_SMS_TIMEOUT_SEND 25000
#define ATDEV_SMS_TIMEOUT_DELALL 10000
#define ATDEV_SMS_TIMEOUT_DEL 8000
#define ATDEV_SMS_TIMEOUT_LIST 5000
#define ATDEV_SMS_TIMEOUT_READ 5000

/**
 * Object for store sms data
 */
class ATData_SMS
{
    public:
        /**
         * Init members with 0
         */
        ATData_SMS();

        /**
         * Init members with params
         *
         * @param number            Pointer to number buffer
         * @param numberSize        Size of number buffer
         * @param message           Pointer to message buffer
         * @param messageSize       Size of message buffer
         */
        ATData_SMS(char *number, uint8_t numberSize, 
                   char *message, uint16_t messageSize);

        /** Phone number string */
        char        *m_number;

        /** Buffer size of number string */
        uint8_t     m_numberSize;

        /** SMS body string */
        char        *m_message;

        /** Buffer size of message string */
        uint16_t    m_messageSize;

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

        /**
         * Initialize the AT device for handle SMS.
         *
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t initializeSMS() = 0;

        /**
         * Send a SMS.
         *
         * @param sms               SMS they will send
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t sendSMS(ATData_SMS *sms) = 0;

        /**
         * Receive SMS from SIM.
         *
         * @param sms               Write SMS data to this object
         * @param idx               Message index SIM store number
         * @return                  ATDEV Okay/Error
         */
        virtual uint8_t receiveSMS(ATData_SMS *sms, uint16_t idx) = 0;

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
         *                          or 0 if you want read from first message
         * @return                  Index nummber or ATDEV_SMS_NO_MSG
         */
        virtual uint16_t readNextIdxSMS(uint16_t lastIdx = 0) = 0;
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
