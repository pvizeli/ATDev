
#ifndef SIM5218_H
#define SIM5218_H

// includes
#include<Arduino.h>

// module on pin
#define SIM5218_ONPIN 2

// chars
#define SIM5218_CR 0x0D
#define SIM5218_LF 0x0A
#define SIM5218_END 0x1A

// commands
#define SIM5218_CMD_AT PSTR("AT")
#define SIM5218_CMD_CPIN PSTR("AT+CPIN=%d")
#define SIM5218_CMD_CREG PSTR("AT+CREG=?")
#define SIM5218_CMD_CGPS PSTR("AT+CGPS=%d,%d")
#define SIM5218_CMD_CGPSINFO PSTR("AT+CGPSINFO")
#define SIM5218_CMD_CPMS PSTR("AT+CPMS=\"SM\",\"SM\",\"SM\"")
#define SIM5218_CMD_CMGS PSTR("AT+CMGS=\"%s\"")
#define SIM5218_CMD_CMGF PSTR("AT+CMGF=%d")

// option
#define SIM5218_OPT_CMGF_TXT 0x01
#define SIM5218_OPT_GPS_ON 0x01
#define SIM5218_OPT_GPS_OFF 0x00
#define SIM5218_OPT_GPS_STD 0x01

// possible ends of AT command
#define SIM5218_END_OK PSTR("OK\x0D\x0A")
#define SIM5218_END_ERROR PSTR("ERROR\x0D\x0A")

// buffer size
// the real size is SIZE+1 for char buffer
#define SIM5218_BUFF_CMD_SIZE 63
#define SIM5218_BUFF_MSG_SIZE 255
#define SIM5218_BUFF_END_SIZE 15

// data size
// the real size is SIZE+1 for char buffer
#define SIM5218_SMS_SIZE 160
#define SIM5218_NUM_SIZE 15
#define SIM5218_LATITUDE_SIZE 15
#define SIM5218_LONGITUDE_SIZE 15
#define SIM5218_ALTITUDE_SIZE 15
#define SIM5218_SPEED_SIZE 8

// error codes
#define SIM5218_ERR_BUFFER_FULL 0x01
#define SIM5218_ERR_ERROR_RECEIVED 0x02
#define SIM5218_ERR_UNEXPECTED_RESULT 0x03
#define SIM5218_ERR_TIMEOUT 0x04
#define SIM5218_ERR_NO_DATA 0x05
#define SIM5218_ERR_UNEXPECTED_EOD 0x06
#define SIM5218_ERR_GPS_CONNECT 0x07
#define SIM5218_ERR_GPS_NOT_INIT 0x08

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
        char m_number[SIM5218_NUM_SIZE + 1];

        /** SMS body string */
        char m_message[SIM5218_SMS_SIZE + 1];

        /**
         * Cleanup all buffers.
         *
         * Call this function to make this object ready for new message.
         */
        void cleanUp();
}

/**
 * Object for store gps data
 */
class SIM5218_gps
{
    private:
        SIM5218_gps() {
            this->cleanUp();
        }

    public:
        char m_latitude[SIM5218_LATITUDE_SIZE];
        char m_longitude[SIM5218_LONGITUDE_SIZE];
        char m_altitude[SIM5218_ALTITUDE_SIZE];
        char m_speed[SIM5218_SPEED_SIZE];

        char m_latPos;
        char m_longPos;

        /**
         * Cleanup all buffers.
         *
         * Call this function to make this object ready for new gps data.
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

        /** Buffer for prepare AT commands they will send */
        char m_cmdBuffer[SIM5218_BUFF_CMD_SIZE + 1];

        /** Answer buffer they fill it after send AT commands */
        char m_msgBuffer[SIM5218_BUFF_MSG_SIZE + 1];

        /** Buffer for handling end of AT request */
        char m_endBuffer[SIM5218_BUFF_END_SIZE + 1];

        /** AT CMD Buffer counter */
        uint16_t m_msgCount;


        // status
        bool m_isGPS;

        /**
         *
         *
         */
        uint8_t sendATCmd(bool defaultEnd, uint16_t timeout);

        /**
         *
         *
         */
        uint8_t sendATCmd() {
            return this->sendATCmd(true, SIM5218_DEFAULT_AT_TIMEOUT);
        }

    public:

        /** SMS Object for receive/send sms */
        SIM5218_sms m_smsObj;

        /** GPS Object for store gps data */
        SIM5218_gps m_gpsObj;

        /**
         *
         *
         */
        bool isReady();

        /**
         *
         */
        uint8_t setPin();

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
         */
        uint8_t updateGPS();

        /**
         *
         *
         */
        uint8_t isReg();

        /**
         *
         */
        uint8_t sendSMS();
}

#endif

# vim: set sts=4 sw=4 ts=4 et:
