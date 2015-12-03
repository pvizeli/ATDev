
#ifndef SIM5218_H
#define SIM5218_H

// module on pin
#define SIM5218_ONPIN 2

// chars
#define SIM5218_CR 0x0D
#define SIM5218_LF 0x0A
#define SIM5218_END 0x1A

// Commands
#define SIM5218_CMD_AT PSTR("AT")
#define SIM5218_CMD_CREG PSTR("AT+CREG=?")
#define SIM5218_CMD_CGPS PSTR("AT+CGPS=%d,%d")
#define SIM5218_CMD_CGPSINFO PSTR("AT+CGPSINFO")
#define SIM5218_CMD_CPMS PSTR("AT+CPMS=\"SM\",\"SM\",\"SM\"")
#define SIM5218_CMD_CMGS PSTR("AT+CMGS=\"%s\"")
#define SIM5218_CMD_CMGF PSTR("AT+CMGF=%d")

// Option
#define SIM5218_OPT_CMGF_TXT 0x01

// Buffer size
// The real size is SIZE+1 for char buffer
#define SIM5218_BUFF_CMD_SIZE 63
#define SIM5218_BUFF_MSG_SIZE 255
#define SIM5218_BUFF_SMS_SIZE 255
#define SIM5218_BUFF_NUM_SIZE 15

// Error codes
#define SIM5218_ERR_BUFFER_FULL 0x01
#define SIM5218_ERR_ERROR_RECEIVED 0x02
#define SIM5218_ERR_UNEXPECTED_RESULT 0x03
#define SIM5218_ERR_TIMEOUT 0x04
#define SIM5218_ERR_NO_DATA 0x05
#define SIM5218_ERR_UNEXPECTED_EOD 0x06

// Time-Outs
#define SIM5218_DEFAULT_AT_TIMEOUT 2500

/**
 * Object for store sms data
 */
class SIM5218_sms
{
    private:
        SIM5218_sms() {
            this->cleanUp();
        }

    public:
        /** Phone number string */
        char m_number[SIM5218_BUFF_NUM_SIZE + 1];

        /** SMS body string */
        char m_message[SIM5218_BUFF_SMS_SIZE + 1];

        /**
         * Cleanup all buffers.
         *
         * Call this function to make this object ready for new message.
         */
        void cleanUp();
}

/**
 * Object for handle all communication with SIM5218 chip
 */
class SIM5218
{
    SIM5218();
    ~SIM5218();

    private:

        // Buffers
        char m_CmdBuffer[SIM5218_BUFF_CMD_SIZE + 1];
        char m_MsgBuffer[SIM5218_BUFF_MSG_SIZE + 1];

        // Buffer counter
        uint16_t m_MsgCount;


        // status
        bool m_isGPS;

        /**
         *
         *
         */
        uint8_t sendATCmd(uint16_t timeout);

        /**
         *
         *
         */
        uint8_t sendATCmd() {
            return this->sendATCmd(SIM5218_DEFAULT_AT_TIMEOUT);
        }

    public:

        /** SMS Object for receive/send sms */
        SIM5218_sms m_smsObj;

        /**
         *
         *
         */
        bool isReady();

        /**
         *
         *
         */
        uint8_t startGPS();

        /**
         *
         *
         */
        uint8_t stopGPS();

        /**
         *
         *
         */
        uint8_t isReg();
}

#endif

# vim: set sts=4 sw=4 ts=4 et:
