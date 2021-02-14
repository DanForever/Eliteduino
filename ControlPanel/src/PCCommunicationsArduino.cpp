#include "PCCommunicationsArduino.h"

#include "Architecture.h"

#ifndef ELITEDUINO_TEENSY

#include <Arduino.h>
#include <HID-Project.h>

void Eliteduino::Hid::Initialize()
{
	const uint8_t bufferSize = 128;
	static uint8_t buffer[ bufferSize ];

	RawHID.begin( buffer, bufferSize );
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

#endif // ELITEDUINO_TEENSY
