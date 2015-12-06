
#ifndef ISMS_H
#define ISMS_H

// data size
// the real size is SIZE+1 for char buffer
#define ATDEV_SMS_TXT_SIZE 160
#define ATDEV_SMS_NUM_SIZE 15

/**
 * Object for store sms data
 */
class SMS_Data
{
    private:
        SMS_Data() {
            this->cleanUp();
        }

    public:
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
}

/**
 * Object for handle all communication with ATDEV chip
 */
class ISMS
{
    ISMS();

    public:

        /** SMS Object for receive/send sms */
        SMS_Data m_smsData;

        /**
         *
         */
        virtual uint8_t sendSMS();
}

#endif

// vim: set sts=4 sw=4 ts=4 et:
