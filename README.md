# ATDev
Arduino library for GSM/3G shield or device with AT commands. It produce very stable and smale code. It also extrem custonable for all sitution with makros.

# Deivce HW

## Arduino
If you want use ATDev with Arduino IDE or any shield functionalitiy you need set macro flag `*_USE_ARDUINO`. Now you have this functions:
- `powerOn` for set a shield on/off power
- `initialize` use this insteat the ATDev. It is provided for HardwareSerial

## List of supported chips

### SIM5218
Option:
- `SIM5218_USE_ARDUINO` for a shield functions or with Arduino IDE compile
- `SIM5218_USE_SMS` for SMS functionable.
- `SIM5218_USE_GPS` for internal AT GPS handling.
