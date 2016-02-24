
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
#define ATDEV_CH_DD 0x3A // :

// commands
#define ATDEV_CMD_AT PSTR("AT")
#define ATDEV_CMD_CPIN PSTR("AT+CPIN=%d")
#define ATDEV_CMD_CREG PSTR("AT+CREG?")

// tools
#define ATDEV_INT_CHAR PSTR("%d")

// possible ends of AT command
#define ATDEV_END_OK PSTR("OK")
#define ATDEV_END_LINE PSTR("\x0D\x0A")
#define ATDEV_END_ERROR PSTR("ERROR")
#define ATDEV_END_ERROR_SIZE 5

// string
#define ATDEV_STR_CMS PSTR("+CMS ERROR")

// buffer size
// the real size is SIZE+1 for char buffer
#define ATDEV_BUFF_CMD_SIZE 48
#define ATDEV_BUFF_END_SIZE 16
#define ATDEV_BUFF_MSG_SIZE 96

// error codes
#define ATDEV_ERR_BUFFER_FULL 0x12
#define ATDEV_ERR_ERROR_RECEIVED 0x13
#define ATDEV_ERR_UNEXPECTED_RESULT 0x14
#define ATDEV_ERR_TIMEOUT 0x15
#define ATDEV_ERR_NO_DATA 0x16
#define ATDEV_ERR_INITIALIZE 0x17
#define ATDEV_ERR_GPS_DATA 0x21
#define ATDEV_ERR_GPS_INIT 0x22

// OK
#define ATDEV_OK 0x01

// Network status
#define ATDEV_NETSTAT_NOT_REGISTERED 0x00
#define ATDEV_NETSTAT_REGISTERED 0x01
#define ATDEV_NETSTAT_SEARCHING 0x02
#define ATDEV_NETSTAT_DENIED 0x03
#define ATDEV_NETSTAT_UNKNOWN 0x04
#define ATDEV_NETSTAT_ROAMING 0x05

// Time-Outs
#define ATDEV_DEFAULT_TIMEOUT 3500
#define ATDEV_FIRST_ATCMD_TIMEOUT 5000
#define ATDEV_POWER_RETRY 10 
#define ATDEV_WAIT 3000
#define ATDEV_FLUSH 500

/**
 * Object for handle all communication with ATDEV chip
 *
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

        /** Possible a pin for power on the shield */
        uint8_t m_onModulePin;

        /** Timeout for sendATCmd */
        uint16_t m_timeOut;

        /**
         * Send/Read a AT formatet string.
         *
         * Is a AT command in cmdBuffer it flush the serial input an 
         * send this AT command. After that it will read the answer
         * until OK or a error receive from modem (default).
         *
         * The Arduino serial buffer are small. It read first all data
         * an parse the input after none data receive from modem. It is
         * possible to break the function immediately if receive a stop
         * string witch the same as in endBuffer.
         *
         * Data are default read to mgsBuffer. You can also use your own
         * input Buffer. EndBuffer hold your stop AT answer. Default use
         * the standard AT answer end command.
         *
         * You can use also the read buffer as stream buffer or circle buffer.
         * It reset the buffer is it full or a line end is detect. So you can
         * search a AT cmd in a stream of data without you have a full buffer
         * error.
         *
         * @param abruptEnd         Stop reading data immediately after
         *                          receive equal data as endBuffer
         * @param streamBuff        Use buffer as circle buf with filtering
         *                          line ends.
         * @param readBuf           Own read buffer insteat msgBuffer.
         * @param readBufSize       Size of his own read buffer.
         * @return                  ATDEV Okay/Error
         */
        uint8_t sendATCmd(bool abruptEnd, bool streamBuff, char* readBuf, uint16_t readBufSize);

        uint8_t sendATCmd() {
            return this->sendATCmd(false, false, m_msgBuffer, ATDEV_BUFF_MSG_SIZE);
        }

        uint8_t sendATCmd(char* readBuf, uint16_t readBufSize) {
            return this->sendATCmd(false, false, readBuf, readBufSize);
        }

        uint8_t sendATCmdAbrupt(char* readBuf, uint16_t readBufSize) {
            return this->sendATCmd(true, false, readBuf, readBufSize);
        }

        uint8_t sendATCmdAbrupt() {
            return this->sendATCmd(true, false, m_msgBuffer, ATDEV_BUFF_MSG_SIZE);
        }

        uint8_t sendATCmdStream() {
            return this->sendATCmd(true, true, m_msgBuffer, ATDEV_BUFF_MSG_SIZE);
        }

        /**
         * Read line from seriel device.
         *
         * @param readBuf           Own read buffer insteat msgBuffer.
         * @param readBufSize       Size of his own read buffer.
         * @return                  ATDEV Okay/Error
         */
        uint8_t readLine(char* readBuf, uint16_t readBufSize);

        uint8_t readLine() {
            return this->readLine(m_msgBuffer, ATDEV_BUFF_MSG_SIZE);
        }

        /**
         * Flush input buffer of AT device.
         */
        void flushInput();

        /**
         * Parse data from msgBuffer with AT characteristic.
         *
         * The function is like strtok but for AT answer. It replace
         * ':' ',' ' ' with 0x00 and you can access to this elemelt with
         * @see getPraseElement. It's like a array.
         *
         * It support string. All data with "bla blu ble" will parse as one
         * element.
         *
         * @param readBuf           Own read buffer insteat msgBuffer.
         * @param readBufSize       Size of his own read buffer.
         * @preturn                 Count of elements that have create
         */
        uint8_t parseInternalData(char* readBuf, uint16_t readBufSize);

        uint8_t parseInternalData() {
            return this->parseInternalData(m_msgBuffer, ATDEV_BUFF_MSG_SIZE);
        }

        /**
         * Get a element of the parsed msgBuffer.
         * @see parseInternalData();
         *
         * @param indx              The element they will have
         * @param readBuf           Own read buffer insteat msgBuffer.
         * @param readBufSize       Size of his own read buffer.
         * @return                  A pointer to this element in msgBuffer
         */
        char* getParseElement(uint8_t indx, char* readBuf, uint16_t readBufSize);

        char* getParseElement(uint8_t indx) {
            return this->getParseElement(indx, m_msgBuffer, ATDEV_BUFF_MSG_SIZE);
        }

        /**
         * Wait function for severail AT commands.
         *
         * If ret is ATDEV_OK it will wait for connection or other
         * things that the device have do. The return is the same
         * as param ret.
         *
         * @param ret               The return command.
         * @return                  @see ret.
         */
        uint8_t waitDevice(uint8_t ret);

        /**
         * Trim AT End from a buffer.
         *
         * @param readBuf           Own read buffer insteat msgBuffer.
         * @param readBufSize       Size of his own read buffer.
         * @param dataSize          Who the last Data in Buffer.
         */
        void trimATEnd(char* readBuf, uint16_t readBufSize, uint16_t dataSize);

        void trimATEnd(char* readBuf, uint16_t readBufSize) {
            this->trimATEnd(readBuf, readBufSize, m_readPtr);
        }

    public:
   
        /**
         * Cleanups
         */
        ATDev();

        /**
         * Initialize this Object.
         *
         * It will set the Serial object for communication and his baudrate.
         * ATDev initialize all system relevant configs.
         *
         * @param UART              Serial object to device
         * @param baudrate          Serial baudrate
         * @param onPinMod          PIN for on/off the shield or 0
         * @return                  ATDEV Okay/Error
         */
        void initialize(HardwareSerial *UART, long baudrate, uint8_t onPinMod);

        /**
         * Power On and check communication to device
         *
         * Is modPin not equal to 0 it will power off and check for AT
         * communication and is the device ready for commands.
         *
         * @return                  ATDEV Okay/Error
         */
        uint8_t onPower();

        /**
         * Send AT command to device for status check.
         * Is the device ready for AT communication?
         *
         * @return                  ATDEV Okay/Error
         */
        uint8_t isReady();

        /**
         * Unlock the SIM.
         *
         * @return                  ATDEV Okay/Error
         */
        uint8_t setSIMPin(uint16_t pin);

        /**
         * Get the network status of the device back.
         *
         * @return                  ATDEV network status or Okay/Error
         */
        uint8_t getNetworkStatus();

        /**
         * Parse a AT+CMS ERROR.
         *
         * @return              True if a error in msgBuffer receive.
         */
        bool isCMSError();
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
