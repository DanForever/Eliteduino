#ifndef __ELITEDUINO_DEVICES_H__
#define __ELITEDUINO_DEVICES_H__

#include "Architecture.h"
#include <stdint.h>

// todo: Move to better location
template<class T, int N>
constexpr int ArraySize( T( & )[ N ] ) { return N; }

// Temporary until proper device configurations are figured out and defined
//#define ELITEDUINO_DEVICE_TEENSY_BUTTONSANDSCREEN
//#define ELITEDUINO_DEVICE_PROMICRO_16BUTTONSONLY

//------------------------------------------------------------------------------------------------

#ifdef ELITEDUINO_DEVICE_PROMICRO_16BUTTONSONLY
#	define ELITEDUINO_BUTTONS
#	define ELITEDUINO_LEDS
#	define ELITEDUINO_LEDMATRIX

const uint8_t BUTTON_COLUMNS[] = { 2, 3, 4, 5 };
const uint8_t BUTTON_ROWS[] = { 6, 7, 8, 9 };
const uint8_t BUTTON_COLUMN_COUNT = ArraySize( BUTTON_COLUMNS );
const uint8_t BUTTON_ROW_COUNT = ArraySize( BUTTON_ROWS );
const uint8_t DEBOUNCE_INTERVAL = 25;

const uint8_t LED_CONTROLLER_PIN_DIN = 10;
const uint8_t LED_CONTROLLER_PIN_CLK = 15;
const uint8_t LED_CONTROLLER_PIN_CS = 14;
const uint8_t LED_CONTROLLER_COLUMN_COUNT = ArraySize( BUTTON_COLUMNS );
const uint8_t LED_CONTROLLER_ROW_COUNT = ArraySize( BUTTON_ROWS );

#endif // ELITEDUINO_DEVICE_PROMICRO_16BUTTONSONLY

//------------------------------------------------------------------------------------------------

#ifdef ELITEDUINO_DEVICE_TEENSY_BUTTONSANDSCREEN

// todo: Actual values
const uint8_t BUTTON_COLUMNS[] = {14};
const uint8_t BUTTON_ROWS[] = {0};
const uint8_t BUTTON_COLUMN_COUNT = ArraySize( BUTTON_COLUMNS );
const uint8_t BUTTON_ROW_COUNT = ArraySize( BUTTON_ROWS );
const uint8_t DEBOUNCE_INTERVAL = 25;

#define ELITEDUINO_SCREEN
#define ELITEDUINO_BUTTONS

#endif // ELITEDUINO_DEVICE_TEENSY_BUTTONSANDSCREEN

#endif // __ELITEDUINO_DEVICES_H__
