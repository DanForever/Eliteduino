#ifndef __ELITEDUINO_LED_CONTROLLER_H__
#define __ELITEDUINO_LED_CONTROLLER_H__

#include <stdint.h>
#include "../PCCommunicationsDefines.h"

class LedControl;

namespace Eliteduino
{
	class PCCommunications;
	struct Message;

	namespace Bindings
	{
		struct Binding;
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

		struct MatrixAddress
		{
			uint8_t Row;
			uint8_t Column;
		};

		struct PinAddress
		{
			uint8_t Index;
		};

		struct Address
		{
			union
			{
				MatrixAddress MatrixAddr;
				PinAddress PinAddr;
			};

			bool IsMatrixAddress : 1;
			bool IsSet : 1;

			Address()
				: IsMatrixAddress(false)
				, IsSet(false)
			{}

			void operator=( MatrixAddress address )
			{
				MatrixAddr = address;
				IsMatrixAddress = true;
				IsSet = true;
			}

			void operator=( PinAddress address )
			{
				PinAddr = address;
				IsMatrixAddress = false;
				IsSet = true;
			}
		};

		enum class AnimationType : uint8_t
		{
			Off,
			BlinkFast,
		};

		struct AnimationData
		{
			unsigned long StartTime;
			AnimationType Type;
			bool Animating = false;
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

			void SetBinding( const MatrixAddress& led, const Bindings::Binding* binding );
			void SetBinding( PinAddress led, const Bindings::Binding* binding );

			void Update();

			void ProcessMessage( const Message& message );

			void ProcessStatReport( const Message& message );

		private:
			void Set( const Address& address, bool on );

			void PlayAnimationOff( eControlRole role );
			void StopAnimation( eControlRole role );

			Matrix* m_matrix = nullptr;
			PCCommunications* m_comms = nullptr;

			Address m_bindings[ (int)eControlRole::Count ];
			AnimationData m_animations[ (int)eControlRole::Count ];
		};
	}
}

#endif // __ELITEDUINO_LED_CONTROLLER_H__
