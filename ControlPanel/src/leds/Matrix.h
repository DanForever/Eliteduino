#ifndef __ELITEDUINO_LED_MATRIX_H__
#define __ELITEDUINO_LED_MATRIX_H__

#include <stdint.h>

class LedControl;

namespace Eliteduino
{
	class PCCommunications;

	namespace Bindings
	{
		union Binding;
	}

	namespace Leds
	{
		class Matrix
		{
		public:
			Matrix() = default;
			~Matrix() = default;

			void Initialize( uint8_t pinDIN, uint8_t pinCLK, uint8_t pinCS, uint8_t numRows, uint8_t numColumns, PCCommunications* comms );

			void Set( uint8_t row, uint8_t column, bool on );
			void SetAll( bool on );

		private:
			LedControl* m_ledController;
			uint8_t m_rowCount;
			uint8_t m_columnCount;
		};
	}
}

#endif // __ELITEDUINO_LED_MATRIX_H__
