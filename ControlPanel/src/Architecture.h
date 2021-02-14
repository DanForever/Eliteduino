#ifndef __ELITEDUINO_ARCHITECTURE_H__
#define __ELITEDUINO_ARCHITECTURE_H__

#if defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY32)
#	define ELITEDUINO_TEENSY
#elif defined(ARDUINO_AVR_PROMICRO) || (defined(ARDUINO_HOODLOADER2)) //todo: figure out a better way of identifying a pro micro when using hid-project
#	define ELITEDUINO_PROMICRO
#endif

#endif 