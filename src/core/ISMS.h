
#ifndef ISMS_H
#define ISMS_H

// include
# include "ATDev.h"

// data size
// the real size is SIZE+1 for char buffer
# ifndef ATDEV_SMS_TXT_SIZE
#  define ATDEV_SMS_TXT_SIZE 160
# endif
# define ATDEV_SMS_NUM_SIZE 15

// option
#define ATDEV_SMS_DEL_ALL 0x04
#define ATDEV_SMS_DEL_READED 0x03

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
         *
         */
        virtual uint8_t initializeSMS() = 0;

        /**
         *
         */
        virtual uint8_t sendSMS() = 0;

        /**
         *
         */
        virtual uint8_t receiveSMS(uint8_t idx) = 0;

        /**
         *
         */
        virtual uint8_t deleteSMS(uint8_t idx) = 0;

        /**
         *
         */
        virtual uint8_t deleteAllSMS(uint8_t flag) = 0;

        /**
         *
         */
        virtual uint8_t readAllNewSMS() = 0;
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
