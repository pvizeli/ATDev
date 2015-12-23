//#define SIM5218_USE_EASYSMS
#define SIM5218_USE_GPS
#define ATDEV_SMS_TXT_SIZE 32

#include <ATDev_HW.h>

SIM5218 modem;

void setup() {

  modem.initialize(&Serial, 115200, 2);
  modem.onPower();

}

void loop() {

  if (modem.isReady() == ATDEV_OK) {

    //modem.initializeSMS();
    
  }

}
