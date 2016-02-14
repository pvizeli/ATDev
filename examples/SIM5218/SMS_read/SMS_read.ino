// Option
#define SIM5218_USE_EASYSMS

#include <ATDev_HW.h>

SIM5218 modem;

// PIN code
uint16_t simPin = 0;

void setup() {
  uint8_t networkStatus = 0;
  uint16_t smsIdx       = 0;

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

        // read all SMS and Delete it
        do {
          smsIdx = modem.readNextIdxSMS();

          // if SMS store index number
          if (smsIdx != ATDEV_SMS_NO_MSG) {

            // read sms into SRAM
            if (modem.receiveSMS(smsIdx) == ATDEV_OK) {
              Serial.print(F("Number: "));
              Serial.println(modem.m_smsData.m_number);
              Serial.println(modem.m_smsData.m_message);
            }
            else {
              Serial.println(F("readSMS fail!"));
            }

            // Delete SMS
            if (modem.deleteSMS(smsIdx) != ATDEV_OK) {
              Serial.println(F("deleteSMS fail!"));
              smsIdx = ATDEV_SMS_NO_MSG;
            }
          }
         
        } while (smsIdx != ATDEV_SMS_NO_MSG);

        // flush all SIM storage
        if (modem.deleteAllSMS(ATDEV_SMS_DEL_ALL) != ATDEV_OK) {
          Serial.println(F("deleteAllSMS fail!"));
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
