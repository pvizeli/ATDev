
#ifndef SIM5218_H
#define SIM5218_H

// includes
#include<ATDev.h>
#include<SMS.h>
#include<GPS.h>

// module on pin
#define SIM5218_ONPIN 2

/**
 * Object for handle all communication with SIM5218 chip
 */
class SIM5218 : public virtual ATDev, public SMS, public GPS
{
    SIM5218();
    ~SIM5218();
}

#endif

# vim: set sts=4 sw=4 ts=4 et:
