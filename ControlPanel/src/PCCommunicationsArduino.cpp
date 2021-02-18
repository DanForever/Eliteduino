#include "PCCommunicationsArduino.h"

#include "Architecture.h"

#ifndef ELITEDUINO_TEENSY

#include <Arduino.h>
#include <HID-Project.h>
#include <stdint.h>


namespace
{
	void InitializeRawHid()
	{
		const uint8_t bufferSize = 128;
		static uint8_t buffer[ bufferSize ];

		RawHID.begin( buffer, bufferSize );
	}

	void InitializeGamepad()
	{
		Gamepad.begin();
	}

	void InitializeKeyboard()
	{
		
	}

	void SendGamepadEvent( Eliteduino::eEventType eventType, uint8_t controlId )
	{
		Serial.println( "Sending gamepad event" );

		switch ( eventType )
		{
		case Eliteduino::eEventType::Press:
			Gamepad.press( controlId );
			break;

		case Eliteduino::eEventType::Release:
			Gamepad.release( controlId );
			break;
		}

		Gamepad.write();
	}

	void SendKeyboardEvent( Eliteduino::eEventType eventType, uint8_t controlId )
	{

	}
}

void Eliteduino::Hid::Initialize()
{
	InitializeRawHid();

	InitializeGamepad();

	InitializeKeyboard();
}

uint8_t Eliteduino::Hid::Recieve( uint8_t* buffer )
{
	const uint8_t bytesToRead = RawHID.available();
	uint8_t bytesRead = 0;

	while ( bytesRead < bytesToRead )
	{
		buffer[ bytesRead ] = RawHID.read();

		++bytesRead;
	}

	return bytesToRead;
}

void Eliteduino::Hid::SendInputEvent( eControlType controlType, eEventType eventType, uint8_t controlId )
{
	switch ( controlType )
	{
	case eControlType::Keyboard:
		SendKeyboardEvent( eventType, controlId );
		break;

	case eControlType::Gamepad:
		SendGamepadEvent( eventType, controlId );
		break;
	}
}

#endif // ELITEDUINO_TEENSY
