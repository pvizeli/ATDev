
#include "IWifi.h"

ATData_WifiAP::ATData_WifiAP()
{
    m_crypt ^= m_crypt;
    m_freq  ^= m_freq;

    memset(m_ssid, 0x00, ATDEV_WIFI_BUF_SSID);
}


// vim: set sts=4 sw=4 ts=4 et:
