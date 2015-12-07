#include <ATDev_HW.h>

SIM5218 modem;

void setup() {

  modem.initialize(&Serial, static_cast<uint16_t>(115200), 2);

}

void loop() {
  

}
