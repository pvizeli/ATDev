
#ifndef ATDEV_H
#define ATDEV_H

// includes
#include<Arduino.h>

// chars
#define ATDEV_CR 0x0D
#define ATDEV_LF 0x0A
#define ATDEV_END 0x1A

// commands
#define ATDEV_CMD_AT PSTR("AT")
#define ATDEV_CMD_CPIN PSTR("AT+CPIN=%d")
#define ATDEV_CMD_CREG PSTR("AT+CREG=?")

// possible ends of AT command
#define ATDEV_END_OK PSTR("OK\x0D\x0A")
#define ATDEV_END_ERROR PSTR("ERROR\x0D\x0A")
#define ATDEV_END_ERROR_SIZE 7

// buffer size
// the real size is SIZE+1 for char buffer
#define ATDEV_BUFF_CMD_SIZE 63
#define ATDEV_BUFF_MSG_SIZE 255
#define ATDEV_BUFF_END_SIZE 15

// error codes
#define ATDEV_ERR_BUFFER_FULL 0x01
#define ATDEV_ERR_ERROR_RECEIVED 0x02
#define ATDEV_ERR_UNEXPECTED_RESULT 0x03
#define ATDEV_ERR_TIMEOUT 0x04
#define ATDEV_ERR_NO_DATA 0x05

// Time-Outs
#define ATDEV_DEFAULT_AT_TIMEOUT 2500

/**
 * Object for handle all communication with ATDEV chip
 */
class ATDev
{
    ATDev();
    ~ATDev();

    protected:

        /** Buffer for prepare AT commands they will send */
        char m_cmdBuffer[ATDEV_BUFF_CMD_SIZE + 1];

        /** Answer buffer they fill it after send AT commands */
        char m_msgBuffer[ATDEV_BUFF_MSG_SIZE + 1];

        /** Buffer for handling end of AT request */
        char m_endBuffer[ATDEV_BUFF_END_SIZE + 1];

        /** AT CMD Buffer counter */
        uint16_t m_msgCount;

        /** Size of end string. Used in sendATCmd */
        uint8_t m_endCount;

        /** Timeout status */
        uint16_t m_timeOutMillis;

        /**
         *
         *
         */
        uint8_t sendATCmd(bool defaultEnd, uint8_t timeOut);

        /**
         *
         *
         */
        uint8_t sendATCmd() {
            return this->sendATCmd(true, ATDEV_DEFAULT_AT_TIMEOUT);
        }

    public:

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
        uint8_t isReg();
}

#endif

# vim: set sts=4 sw=4 ts=4 et:
