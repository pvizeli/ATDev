
#ifndef ESP8266_H
#define ESP8266_H

#include "../ATDev_HW.h"

// commands
#define ATDEV_CMD_CWMODE_CUR PSTR("AT+CWMODE_CUR=%d")
#define ATDEV_CMD_CWMODE_CUR? PSTR("AT+CWMODE_CUR?")
#define ATDEV_CMD_CWJAP_CUR PSTR("AT+CWJAP_CUR=\"%s\",\"%s\"")
#define ATDEV_CMD_CWLAP PSTR("AT+CWLAP")
#define ATDEV_CMD_CWQAP PSTR("AT+CWQAP")
#define ATDEV_CMD_CWDHCP_CUR PSTR("AT+CWDHCP_CUR=%d,%d")
#define ATDEV_CMD_CIPSTA_CUR PSTR("AT+CIPSTA_CUR=\"%s\",\"%s\",\"%s\"")
#define ATDEV_CMD_CIFSR PSTR("AT+CIFSR")
#define ATDEV_CMD_RST PSTR("AT+RST")
#define ATDEV_CMD_SLEEP PSTR("AT+SLEEP=%d")

// option
#define ESP8266_OPT_MODE_STATION 0x01
#define ESP8266_OPT_MODE_SOFTAP 0x02
#define ESP8266_OPT_MODE_SOFTAP-STATION 0x02
#define ESP8266_OPT_DHCP_ON 0x01
#define ESP8266_OPT_DHCP_OFF 0x00

/**
 * Object for handle GPS communication with SIM5218 chip
 */
class _ESP8266_WifiClient : 
    public virtual ATDev,
    public IWifiClient
{
    protected :

        uint8_t restartChip();

    public:
        /**
         * Implement from @see IWifiClient
         */

        virtual uint8_t initializeWifiClient();

        virtual uint8_t connect(char *ssid, char *key);

        virtual uint8_t disconnect();

        virtual uint8_t getIP(char *ip, uint8_t ipSize);

        virtual uint8_t setIP(char *ip, char *gateway, char *netmask);

        virtual uint8_t setDHCP();

        virtual uint8_t getWifiStatus();

        virtual uint8_t scanWifi(ATData_WifiAP *list, uint8_t count);

        virtual uint8_t setSleep(uint8_t mod);
};

/**
 * Object for handle all communication with SIM5218 chip
 */
class ESP8266 : 
#ifdef ESP8266_USE_ARDUINO
    ,public ATArduino
#endif
#ifdef ESP8266_WIFICLIENT
    ,public _ESP8266_WIFICLIENT
#endif
{

};

#endif

// vim: set sts=4 sw=4 ts=4 et:
