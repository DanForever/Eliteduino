#ifndef __ELITEDUINO_DEVICES_H__
#define __ELITEDUINO_DEVICES_H__

#include "Architecture.h"

// Temporary until proper device configurations are figured out and defined
#define ELITEDUINO_SCREEN

#ifdef ELITEDUINO_DEVICE_PROMICRO_16BUTTONSONLY
#	define ELITEDUINO_DEVICE_BUTTON_ROWS 4
#	define ELITEDUINO_DEVICE_BUTTON_COLUMNS 4
#	define ELITEDUINO_DEVICE_BUTTON_COLUMN_PINS { 2, 3, 4, 5 }
#	define ELITEDUINO_DEVICE_BUTTON_ROW_PINS { 6, 7, 8, 9 }

#	define ELITEDUINO_BUTTONS
#endif

#ifdef ELITEDUINO_BUTTONS

namespace Buttons
{
	struct
	{
		uint8_t ColumnPins[ ELITEDUINO_DEVICE_BUTTON_COLUMNS ];
		uint8_t RowPins[ ELITEDUINO_DEVICE_BUTTON_ROWS ];
	} Config = { ELITEDUINO_DEVICE_BUTTON_COLUMN_PINS, ELITEDUINO_DEVICE_BUTTON_ROW_PINS };
}

#endif // ELITEDUINO_BUTTONS

#ifdef ELITEDUINO_SCREEN



#endif // ELITEDUINO_SCREEN

#endif // __ELITEDUINO_DEVICES_H__