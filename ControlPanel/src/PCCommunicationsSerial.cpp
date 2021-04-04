#include "PCCommunicationsSerial.h"

#include <Arduino.h>

void Eliteduino::Serial::Initialize()
{
	::Serial.begin( 9600 );
}

uint8_t Eliteduino::Serial::Recieve( uint8_t* buffer )
{
	const uint8_t bytesToRead = ::Serial.available();
	uint8_t bytesRead = 0;

	while ( bytesRead < bytesToRead )
	{
		buffer[ bytesRead ] = ::Serial.read();

		++bytesRead;
	}

	return bytesToRead;
}
