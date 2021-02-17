#include "../Devices.h"

#ifdef ELITEDUINO_BUTTONS

#include "Button.h"

void Eliteduino::Controls::Button::Initialize( uint8_t pin, uint8_t pressedState )
{
	m_button.attach( pin, INPUT_PULLUP );

	m_button.setPressedState( pressedState );
}

void Eliteduino::Controls::Button::SetDebounceInterval( int milliseconds )
{
	m_button.interval( milliseconds );
}

void Eliteduino::Controls::Button::Update()
{
	m_button.update();

	if ( m_button.pressed() )
	{
		Serial.println( "button pressed" );
	}
}

#endif // ELITEDUINO_BUTTONS
