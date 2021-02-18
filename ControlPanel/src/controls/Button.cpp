#include "../Devices.h"

#ifdef ELITEDUINO_BUTTONS

#include "Button.h"

#include "../PCCommunications.h"
#include "../PCCommunicationsDefines.h"

void Eliteduino::Controls::Button::Initialize( uint8_t pin, uint8_t pressedState, PCCommunications* comms )
{
	m_comms = comms;

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
		Serial.println( "Button pressed" );
		m_comms->SendInputEvent( eControlType::Gamepad, eEventType::Press, 2 );
	}
	else if ( m_button.released() )
	{
		Serial.println( "Button released" );

		m_comms->SendInputEvent( eControlType::Gamepad, eEventType::Release, 2 );
	}
}

#endif // ELITEDUINO_BUTTONS
