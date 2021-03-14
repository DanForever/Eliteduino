#include "../Devices.h"

#ifdef ELITEDUINO_BUTTONS

#include "Button.h"

#include "../bindings/Binding.h"
#include "../debug/Debug.h"
#include "../PCCommunications.h"
#include "../PCCommunicationsDefines.h"

void Eliteduino::Controls::Button::Initialize( uint8_t pin, uint8_t pressedState, PCCommunications* comms )
{
	m_comms = comms;

	m_button.attach( pin, INPUT_PULLUP );

	SetNormallyOpen( true );
}

void Eliteduino::Controls::Button::SetDebounceInterval( int milliseconds )
{
	m_button.interval( milliseconds );
}

void Eliteduino::Controls::Button::SetNormallyOpen( bool isOpen )
{
	uint8_t buttonIsPressedWhenPinIs = isOpen ? LOW : HIGH;
	m_button.setPressedState( buttonIsPressedWhenPinIs );
}

void Eliteduino::Controls::Button::Update()
{
	if ( m_button.update() )
	{
		switch ( m_binding->PhysicalType )
		{
		case ePhysicalControlType::Momentary:
			if ( m_button.pressed() )
			{
				PRINT( "Momentary button pressed: ", (int)m_binding->VirtualType, " / ", m_binding->Value );
				m_comms->SendInputEvent( m_binding->VirtualType, eEventType::Press, m_binding->Value );
			}
			else if ( m_button.released() )
			{
				PRINT( "Momentary button released: ", (int)m_binding->VirtualType, " / ", m_binding->Value );
				m_comms->SendInputEvent( m_binding->VirtualType, eEventType::Release, m_binding->Value );
			}
			break;

		case ePhysicalControlType::Toggle:
			PRINT( "Toggle button changed: ", (int)m_binding->VirtualType, " / ", m_binding->Value );
			m_comms->SendInputEvent( m_binding->VirtualType, eEventType::Press, m_binding->Value );
			m_comms->SendInputEvent( m_binding->VirtualType, eEventType::Release, m_binding->Value );
			break;
		}
	}
}

#endif // ELITEDUINO_BUTTONS
