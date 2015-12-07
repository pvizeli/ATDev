
#ifndef ATDEV_H
#define ATDEV_H

// includes
#include <Arduino.h>
#include <inttypes.h>
#include <string.h>
#include <avr/pgmspace.h>

// chars
#define ATDEV_CR 0x0D
#define ATDEV_LF 0x0A
#define ATDEV_END 0x1A

// commands
#define ATDEV_CMD_AT PSTR("AT")
#define ATDEV_CMD_CPIN PSTR("AT+CPIN=%d")
#define ATDEV_CMD_CREG PSTR("AT+CREG=?")

// possible ends of AT command
#define ATDEV_END_OK PSTR("OK")
#define ATDEV_END_ERROR PSTR("ERROR")
#define ATDEV_END_ERROR_SIZE 5

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
#define ATDEV_POWER_TIMEOUT 5000

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

        /** AT CMD Buffer counter */
        uint16_t m_msgPtr;

        /** Size of end string. Used in sendATCmd */
        uint8_t m_endSize;

        /** */
        uint8_t m_onModulePin;

        /** Timeout status */
        uint16_t m_timeOutMillis;

        /**
         *
         *
         */
        uint8_t sendATCmd(uint16_t timeOut = ATDEV_DEFAULT_TIMEOUT, bool abruptEnd = false);

    public:
    
        ATDev();


        uint8_t initialize(HardwareSerial *UART, uint16_t baudrate, uint8_t onPinMod);

        /**
         *
         *
         */
        uint8_t onPower(uint16_t timeOut = ATDEV_POWER_TIMEOUT);

        /**
         *
         *
         */
        uint8_t isReady(uint16_t timeOut = ATDEV_DEFAULT_TIMEOUT);

        /**
         *
         */
        //uint8_t setSIMPin(uint16_t timeOut = ATDEV_DEFAULT_TIMEOUT);

        /**
         *
         *
         */
        //uint8_t getNetworkStatus(uint16_t timeOut = ATDEV_DEFAULT_TIMEOUT);
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
