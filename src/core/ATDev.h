
#ifndef ATDEV_H
#define ATDEV_H

// includes
#include <Arduino.h>
#include <inttypes.h>
#include <string.h>
#include <avr/pgmspace.h>

// chars
#define ATDEV_CH_CR 0x0D
#define ATDEV_CH_LF 0x0A
#define ATDEV_CH_END 0x1A
#define ATDEV_CH_SP 0x20
#define ATDEV_CH_CO 0x2C // ,
#define ATDEV_CH_IC 0x22 // "
#define ATDEV_CH_AN 0x3E // >

// commands
#define ATDEV_CMD_AT PSTR("AT")
#define ATDEV_CMD_CPIN PSTR("AT+CPIN=%d")
#define ATDEV_CMD_CREG PSTR("AT+CREG=?")

// possible ends of AT command
#define ATDEV_END_OK PSTR("OK")
#define ATDEV_END_ERROR PSTR("ERROR")
#define ATDEV_END_ERROR_SIZE 5
#define ATDEV_END_CMS PSTR("+CMS ERROR")

// buffer size
// the real size is SIZE+1 for char buffer
#define ATDEV_BUFF_CMD_SIZE 32
#define ATDEV_BUFF_MSG_SIZE 64
#define ATDEV_BUFF_END_SIZE 16

// error codes
#define ATDEV_ERR_BUFFER_FULL 0x12
#define ATDEV_ERR_ERROR_RECEIVED 0x13
#define ATDEV_ERR_UNEXPECTED_RESULT 0x14
#define ATDEV_ERR_TIMEOUT 0x15
#define ATDEV_ERR_NO_DATA 0x16
#define ATDEV_ERR_INITIALIZE 0x17

// OK
#define ATDEV_OK 0x01

// Network status
#define ATDEV_NETSTAT_UNKNOWN 0x00
#define ATDEV_NETSTAT_DENIED 0x01
#define ATDEV_NETSTAT_NOT_REGISTERED 0x02
#define ATDEV_NETSTAT_SEARCHING 0x03
#define ATDEV_NETSTAT_ROAMING 0x04
#define ATDEV_NETSTAT_REGISTERED 0x05

// Time-Outs
#define ATDEV_DEFAULT_TIMEOUT 2500
#define ATDEV_POWER_RETRY 4 

/**
 * Object for handle all communication with ATDEV chip
 */
class ATDev
{
    protected:

        /** UART inteface for communication with device */
        HardwareSerial* m_hwSerial;

        /** Buffer for prepare AT commands they will send */
        char m_cmdBuffer[ATDEV_BUFF_CMD_SIZE + 1];

        /** Answer buffer they fill it after send AT commands */
        char m_msgBuffer[ATDEV_BUFF_MSG_SIZE + 1];

        /** Buffer for handling end of AT request */
        char m_endBuffer[ATDEV_BUFF_END_SIZE + 1];

        /** AT CMD read Buffer counter */
        uint16_t m_readPtr;

        /** */
        uint8_t m_onModulePin;

        /** Timeout for sendATCmd */
        uint16_t m_timeOut;

        /**
         *
         *
         */
        uint8_t sendATCmd(bool abruptEnd = false, char* readBuf = NULL, uint16_t readBufSize = ATDEV_BUFF_MSG_SIZE);

        /**
         *
         *
         */
        uint8_t parseInternalData();

        /**
         *
         *
         */
        char* getParseElement(uint8_t indx);

        /**
         *
         */
        bool isCMSError();

    public:
    
        ATDev();


        void initialize(HardwareSerial *UART, long baudrate, uint8_t onPinMod);

        /**
         *
         *
         */
        uint8_t onPower();

        /**
         *
         *
         */
        uint8_t isReady();

        /**
         *
         */
        uint8_t setSIMPin(uint16_t pin);

        /**
         *
         *
         */
        uint8_t getNetworkStatus();
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
