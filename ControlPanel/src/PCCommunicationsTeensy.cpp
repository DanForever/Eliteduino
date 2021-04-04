#include "PCCommunicationsTeensy.h"

#include "Architecture.h"

#ifdef ELITEDUINO_TEENSY

#include <Arduino.h>
#include "debug/Debug.h"

namespace
{
	void SendKeyboardEvent( Eliteduino::eEventType eventType, ControlCode controlId )
	{
		PRINT( "Sending keyboard event (", (int)eventType, "/", controlId, ")" );

		switch ( eventType )
		{
		case Eliteduino::eEventType::Press:
			Keyboard.press( controlId );
			break;

		case Eliteduino::eEventType::Release:
			Keyboard.release( controlId );
			break;
		}
	}
}

void Eliteduino::Hid::Initialize()
{

}

uint8_t Eliteduino::Hid::Recieve( uint8_t* buffer )
{
	constexpr int DO_NOT_WAIT = 0;
	return RawHID.recv( buffer, DO_NOT_WAIT );
}

void Eliteduino::Hid::SendInputEvent( eVirtualControlType controlType, eEventType eventType, ControlCode controlId )
{
	switch ( controlType )
	{
	case eVirtualControlType::Keyboard:
		SendKeyboardEvent( eventType, controlId );
		break;

	case eVirtualControlType::Gamepad:
		//SendGamepad1Event( eventType, controlId );
		break;
	}
}

#endif // ELITEDUINO_TEENSY
