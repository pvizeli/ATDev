
#ifndef IWIFI_H
#define IWIFI_H

// include
# include "ATDev.h"

// buffer size
// the real size is SIZE+1 for char buffer
#define ATDEV_WIFI_BUF_SSID 20

// wifi encription code
#define ATDEV_WIFI_CRYPT_OPEN 0x01
#define ATDEV_WIFI_CRYPT_WEB 0x02
#define ATDEV_WIFI_CRYPT_WPA_PSK 0x03
#define ATDEV_WIFI_CRYPT_WPA2_PSK 0x04
#define ATDEV_WIFI_CRYPT_WPA-2_PSK 0x05

// Wifi stats
#define ATDEV_WIFI_STAT_CONNECTED 0x01
#define ATDEV_WIFI_STAT_DISCONNECTED 0x02
#define ATDEV_WIFI_STAT_LOST 0x03
#define ATDEV_WIFI_STAT_SLEEP 0x04

#define ATDEV_WIFI_SLEEP_OFF 0x00
#define ATDEV_WIFI_SLEEP_LOW 0x01
#define ATDEV_WIFI_SLEEP_DEEP 0x02

// Timeouts
#define ATDEV_WIFI_TIMEOUT_CONNECT 15000
#define ATDEV_WIFI_TIMEOUT_RESTART 15000

/**
 * Object for handle a view 
 */
class ATData_WifiAP
{
    pubic:
    
        /**
         * Cleanup member data
         */
        ATData_WifiAP();

        /** */
        char    m_ssid[ATDEV_WIFI_BUF_SSID +1];

        /** */
        uint8_t m_crypt;
        
        /** */
        int8_t  m_freq;
};

/**
 * Object for handle all communication with ATDEV chip
 */
class IWifiClient
{
    public:
        
        /**
         *
         */
        virtual uint8_t initializeWifiClient() = 0;

        /**
         *
         */
        virtual uint8_t connect(char *ssid, char *key) = 0;

        /**
         *
         */
        virtual uint8_t disconnect() = 0;

        /**
         *
         */
        virtual uint8_t getIP(char *ip, uint8_t ipSize) = 0;

        /**
         *
         */
        virtual uint8_t setIP(char *ip, char *gateway, char *netmask) = 0;

        /**
         *
         */
        virtual uint8_t setDHCP() = 0;

        /**
         *
         */
        virtual uint8_t getWifiStatus() = 0;

        /**
         *
         */
        virtual uint8_t scanWifi(ATData_WifiAP *list, uint8_t count) = 0;

        /**
         *
         */
        virtual uint8_t setSleep(uint8_t mod) = 0;
};


#endif

// vim: set sts=4 sw=4 ts=4 et:
