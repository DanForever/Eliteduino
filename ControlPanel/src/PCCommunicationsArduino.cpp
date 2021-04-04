#include "PCCommunicationsArduino.h"

#ifndef ELITEDUINO_TEENSY

#include <Arduino.h>
#include <HID-Project.h>
#include <stdint.h>
#include "debug/Debug.h"

namespace
{
	void InitializeRawHid()
	{
		const uint8_t bufferSize = 128;
		static uint8_t buffer[ bufferSize ];

		PRINT( "Debug RawHid" );

		RawHID.begin( buffer, bufferSize );

		RawHID.debugPrintHidInfo();
	}

	void InitializeGamepad()
	{
		PRINT( "Debug Gamepad" );

		Gamepad1.begin();

		Gamepad1.debugPrintHidInfo();
	}

	void InitializeKeyboard()
	{
		SingleNKROKeyboard.begin();
	}

	void SendGamepad1Event( Eliteduino::eEventType eventType, ControlCode controlId )
	{
		PRINT( "Sending Gamepad1 event (", (int)eventType, "/", controlId, ")" );

		switch ( eventType )
		{
		case Eliteduino::eEventType::Press:
			Gamepad1.press( controlId );
			break;

		case Eliteduino::eEventType::Release:
			Gamepad1.release( controlId );
			break;
		}

		Gamepad1.write();
	}

	void SendKeyboardEvent( Eliteduino::eEventType eventType, ControlCode controlId )
	{
		PRINT( "Sending keyboard event (", (int)eventType, "/", controlId, ")" );

		switch ( eventType )
		{
		case Eliteduino::eEventType::Press:
			SingleNKROKeyboard.add( (KeyboardKeycode)controlId );
			break;

		case Eliteduino::eEventType::Release:
			SingleNKROKeyboard.remove( (KeyboardKeycode)controlId );
			break;
		}

		SingleNKROKeyboard.send();
	}
}

void Eliteduino::Hid::Initialize()
{
	PRINT( "Eliteduino::Hid::Initialize" );

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

void Eliteduino::Hid::SendInputEvent( eVirtualControlType controlType, eEventType eventType, ControlCode controlId )
{
	switch ( controlType )
	{
	case eVirtualControlType::Keyboard:
		SendKeyboardEvent( eventType, controlId );
		break;

	case eVirtualControlType::Gamepad:
		SendGamepad1Event( eventType, controlId );
		break;
	}
}

#endif // ELITEDUINO_TEENSY
