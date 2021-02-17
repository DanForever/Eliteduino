#ifndef __ELITEDUINO_BUTTON_H__
#define __ELITEDUINO_BUTTON_H__

#include <Arduino.h>
#include <Bounce2.h>

namespace Eliteduino
{
	namespace Controls
	{
		class Button
		{
		public:
			Button() = default;
			~Button() = default;

			void Initialize( uint8_t pin, uint8_t pressedState );
			void SetDebounceInterval( int miliseconds );

			void Update();

			bool Raw() const { return m_button.read(); }

		private:
			Bounce2::Button m_button;
		};
	}
}

#endif // __ELITEDUINO_BUTTON_H__
