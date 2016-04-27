
#define SIM5218_USE_ARDUINO

#include <ATDev_HW.h>

SIM5218 modem;

// PIN code
uint16_t simPin = 0;

void setup() {
  uint8_t networkStatus = 0;

  // Device data initialize
  modem.initialize(&Serial, 115200);

  // Power on the Shield && init
  if (modem.onPower(2) == ATDEV_OK) {

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
