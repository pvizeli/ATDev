// Option
#define SIM5218_USE_GPS

#include <ATDev_HW.h>

SIM5218 modem;

#define COUNT_RECEIVE 10
#define TIME_WAIT 20000

void setup() {
  uint8_t gpsRet = 0;
  
  // Device data initialize
  modem.initialize(&Serial, 115200, 2);

  // Power on the Shield && init
  if (modem.onPower() == ATDEV_OK) {

    // Init GPS
    if (modem.initializeGPS() == ATDEV_OK) {

      // Start GPS on device
      if (modem.onGPS() == ATDEV_OK) {

        // receive GPS Data
        for (uint8_t i = 0; i < COUNT_RECEIVE; i++) {

          // load data
          gpsRet = modem.receiveGPS();

          if (gpsRet == ATDEV_OK) {
            Serial.println(F("Load GPS data:"));

            Serial.print(F("Latitude: "));
            Serial.println(modem.m_gpsData.m_latitude);
            Serial.print(F("Longitude: "));
            Serial.println(modem.m_gpsData.m_longitude);
            Serial.print(F("Altitude: "));
            Serial.println(modem.m_gpsData.m_altitude);
            Serial.print(F("Speed: "));
            Serial.println(modem.m_gpsData.m_speed);
            Serial.print(F("Date: "));
            Serial.println(modem.m_gpsData.m_date);
            Serial.print(F("Time: "));
            Serial.println(modem.m_gpsData.m_time);

            // Google Map link
            Serial.print(F("https://www.google.com/maps?q="));
            Serial.print(modem.m_gpsData.m_latitude);
            Serial.print(F(","));
            Serial.println(modem.m_gpsData.m_longitude);
          }
          else if (gpsRet == ATDEV_ERR_GPS_DATA) {
            Serial.println(F("No GPS connection / data"));
          }
          else {
            Serial.println(F("receiveGPS fails!"));
          }

          // wait for next update
          delay(TIME_WAIT);
        }

        // Stop GSP Session
        if (modem.offGPS() != ATDEV_OK) {
          Serial.println(F("offGPS fails!"));
        }
      }
      else {
        Serial.println(F("onGPS fails!"));
      }
    }
    else {
      Serial.println(F("initializeGPS fails!"));
    }
  }
  else {
    Serial.println(F("PowerOn fails!"));
  }

}

void loop() {
  

}

