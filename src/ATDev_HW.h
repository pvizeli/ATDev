
#ifndef ATDEV_HW_H
#define ATDEV_HW_H

// default size for helping
#define ATDEV_DEFAULT_SMSTXT_SIZE 166 // 160 char, 5 AT stuff and 1 for 0x00
#define ATDEV_DEFAULT_NUM_SIZE 20

// core
#include "core/ATDev.h"
#include "core/ATAsync.h"

// interface
#include "interface/ISMS.h"
#include "interface/IGPS.h"
#include "interface/IWifi.h"

// module
#include "module/ATCarrier.h"
#include "module/ATSMS.h"

// platform
#include "platform/ATArduino.h"

// misc


// Hardware
#include "hardware/SIM5218.h"

#endif

