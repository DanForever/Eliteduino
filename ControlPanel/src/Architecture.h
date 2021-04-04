#ifndef __ELITEDUINO_ARCHITECTURE_H__
#define __ELITEDUINO_ARCHITECTURE_H__

#include <stdint.h>

#if defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY32)

// Platform defines
#	define ELITEDUINO_TEENSY
#	define ELITEDUINO_PC_COMMS_HID

// Platform types
using ControlCode = uint16_t;

#elif defined(ARDUINO_AVR_PROMICRO)

// Platform defines
#	define ELITEDUINO_PROMICRO
#	define ELITEDUINO_PC_COMMS_SERIAL

// Platform types
using ControlCode = uint8_t;
#endif

#endif // __ELITEDUINO_ARCHITECTURE_H__
