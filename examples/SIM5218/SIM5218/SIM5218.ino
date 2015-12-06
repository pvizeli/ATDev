#include <ATDev_HW.h>

SIM5218 modem;

void setup() {

  modem.initialize(&Serial, 115200, 2);

}

void loop() {
  

}
