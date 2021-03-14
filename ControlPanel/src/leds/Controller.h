#ifndef __ELITEDUINO_LED_CONTROLLER_H__
#define __ELITEDUINO_LED_CONTROLLER_H__

#include <stdint.h>

class LedControl;

namespace Eliteduino
{
	class PCCommunications;

	namespace Bindings
	{
		union Binding;
	}

	struct MatrixDimensions
	{
		uint8_t RowCount;
		uint8_t ColumnCount;
	};

	namespace Leds
	{
		// If you have a matrix of LEDs powered by a Max72* chip
		struct LedMatrixConfig
		{
			struct
			{
				uint8_t Din;
				uint8_t Clk;
				uint8_t Cs;
			} Pin;

			MatrixDimensions Dimensions;
		};

		// If you have connected up LEDs directly to the microcontroller pins
		struct LedPinConfig
		{
			uint8_t* Pin = nullptr;
			uint8_t Count = 0;
		};

		class Matrix;

		// This class is meant to encapsulate the various different methods of addressing LEDs your control panel may have
		// (i.e. LED Matrix + some individually controlled LEDs directly on pins
		class Controller
		{
		public:
			Controller() = default;
			~Controller() = default;

			void Initialize( const LedMatrixConfig& matrix, PCCommunications* comms );
			void Initialize( const LedPinConfig& pins, PCCommunications* comms );
			void Initialize( const LedMatrixConfig& matrix, const LedPinConfig& pins, PCCommunications* comms );

			void SetBinding( uint8_t index, const Bindings::Binding* binding );

			void Update();

		private:
			Matrix* m_matrix = nullptr;
		};
	}
}

#endif // __ELITEDUINO_LED_CONTROLLER_H__
