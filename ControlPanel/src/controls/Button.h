#ifndef __ELITEDUINO_BUTTON_H__
#define __ELITEDUINO_BUTTON_H__

#include <Arduino.h>
#include <Bounce2.h>

namespace Eliteduino
{
	class PCCommunications;

	namespace Bindings
	{
		union Binding;
	}

	namespace Controls
	{
		class Button
		{
		public:
			Button() = default;
			~Button() = default;

			void Initialize( uint8_t pin, uint8_t pressedState, PCCommunications* comms );
			void SetDebounceInterval( int miliseconds );
			void SetBinding( const Bindings::Binding* binding ) { m_binding = binding; }
			void SetNormallyOpen( bool isOpen );

			void Update();

			bool Raw() const { return m_button.read(); }

		private:
			Bounce2::Button m_button;
			PCCommunications* m_comms = nullptr;
			const Bindings::Binding* m_binding = nullptr;
		};
	}
}

#endif // __ELITEDUINO_BUTTON_H__
