// Option
#define SIM5218_USE_EASYSMS

#include <ATDev_HW.h>

SIM5218 modem;

// Text & data
#define MY_SMS_BODY PSTR("Hello world! That is my SMS :P")
#define MY_SMS_NUMBER PSTR("")

// PIN code
uint16_t simPin = 0;

void setup() {
  uint8_t networkStatus = 0;

  // Device data initialize
  modem.initialize(&Serial, 115200, 2);

  // Power on the Shield && init
  if (modem.onPower() == ATDEV_OK) {

    // set PIN is avilable
    if (simPin > 0) {
      if (modem.setSIMPin(simPin) != ATDEV_OK) {
        Serial.println(F("SIMPin fails!"));
      }
    }

    // Check Network Status
    networkStatus = modem.getNetworkStatus();

    // Connected with Carrier
    if (networkStatus == ATDEV_NETSTAT_REGISTERED || networkStatus == ATDEV_NETSTAT_ROAMING) {
      Serial.println(F("Modem connected with carrier"));

      if (modem.initializeSMS() == ATDEV_OK) {
        // Prepare sms data
        strncpy_P(modem.m_smsData.m_number, MY_SMS_NUMBER, ATDEV_SMS_NUM_SIZE);
        strncpy_P(modem.m_smsData.m_message, MY_SMS_BODY, ATDEV_SMS_TXT_SIZE);
  
        // send SMS
        if (modem.sendSMS() == ATDEV_OK) {
          Serial.println(F("SMS send"));
        }
        else {
          Serial.println(F("sendSMS fail!"));
        }
      }
      else {
        Serial.println(F("initializeSMS fail!"));
      }
    }
    else {
      Serial.print(F("Modem not connected! State: "));
      Serial.println(networkStatus, HEX);
    }
  }
  else {
    Serial.println(F("PowerOn fails!"));
  }

}

void loop() {
  

}
