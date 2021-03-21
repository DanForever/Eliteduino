#ifndef __ELITEDUINO_MATRIX_UTILITY_H__
#define __ELITEDUINO_MATRIX_UTILITY_H__

#include <stdint.h>

namespace Eliteduino
{
	inline uint8_t MatrixIndex( uint8_t row, uint8_t column, uint8_t rowCount )
	{
		return row + ( column * rowCount );
	}

	inline uint8_t MatrixRow( uint8_t index, uint8_t rowCount )
	{
		return index / rowCount;
	}

	inline uint8_t MatrixColumn( uint8_t index, uint8_t rowCount )
	{
		return index % rowCount;
	}
}

#endif // __ELITEDUINO_MATRIX_UTILITY_H__
