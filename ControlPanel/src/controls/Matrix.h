#ifndef __ELITEDUINO_CONTROL_MATRIX_H__
#define __ELITEDUINO_CONTROL_MATRIX_H__

#include <stdint.h>

namespace Eliteduino
{
	class PCCommunications;

	namespace Bindings
	{
		struct Binding;
	}

	namespace Controls
	{
		class Button;

		class Matrix
		{
		public:
			Matrix() = default;
			~Matrix() = default;

			void Initialize( const uint8_t* rows, const uint8_t* columns, uint8_t numRows, uint8_t numCols, int debounceInterval, PCCommunications* comms );

			void SetNormallyOpen( uint8_t index, bool isOpen );
			void SetNormallyOpen( uint8_t row, uint8_t column, bool isOpen );
			void SetBinding( uint8_t index, const Bindings::Binding* binding );
			void SetBinding( uint8_t row, uint8_t column, const Bindings::Binding* binding );

			void Update();

		private:
			void ActivateColumn( uint8_t column );
			void DeactivateColumn( uint8_t column );

			Button* m_buttons;
			const uint8_t* m_rows = nullptr;
			const uint8_t* m_columns = nullptr;
			uint8_t m_rowCount = 0;
			uint8_t m_columnCount = 0;
		};
	}
}

#endif // __ELITEDUINO_CONTROL_MATRIX_H__
