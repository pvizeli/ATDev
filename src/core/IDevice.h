
#ifndef IDEVICE_H
#define IDEVICE_H

/**
 * Interface for all hardware class with use in arduino
 */
class IDevice
{
    public:

        /**
         *
         */
        virtual uint8_t initialize(HardwareSerial *UART, uint8_t baudrate) = 0;
}

#endif

# vim: set sts=4 sw=4 ts=4 et:
