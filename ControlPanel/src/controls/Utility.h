#ifndef __ELITEDUINO_CONTROLS_UTILITY_H__
#define __ELITEDUINO_CONTROLS_UTILITY_H__

#include <stdint.h>

namespace Eliteduino
{
	namespace Controls
	{
		inline uint8_t ButtonIndex( uint8_t row, uint8_t column, uint8_t rowCount )
		{
			return row + ( column * rowCount );
		}

		inline uint8_t ButtonRow( uint8_t index, uint8_t rowCount )
		{
			return index / rowCount;
		}

		inline uint8_t ButtonColumn( uint8_t index, uint8_t rowCount )
		{
			return index % rowCount;
		}
	}
}

#endif // __ELITEDUINO_CONTROLS_UTILITY_H__
