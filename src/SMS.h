
#ifndef SMS_H
#define SMS_H

// includes
#include<ATDev.h>

// commands
#define ATDEV_CMD_CPMS PSTR("AT+CPMS=\"SM\",\"SM\",\"SM\"")
#define ATDEV_CMD_CMGS PSTR("AT+CMGS=\"%s\"")
#define ATDEV_CMD_CMGF PSTR("AT+CMGF=%d")

// option
#define ATDEV_OPT_CMGF_TXT 0x01

// data size
// the real size is SIZE+1 for char buffer
#define SMS_TXT_SIZE 160
#define SMS_NUM_SIZE 15

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
        char m_number[SMS_NUM_SIZE + 1];

        /** SMS body string */
        char m_message[SMS_TXT_SIZE + 1];

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
class SMS : public virtual ATDev
{
    SMS();
    ~SMS();

    public:

        /** SMS Object for receive/send sms */
        SMS_Data m_smsData;

        /**
         *
         */
        uint8_t sendSMS();
}

#endif

# vim: set sts=4 sw=4 ts=4 et:
