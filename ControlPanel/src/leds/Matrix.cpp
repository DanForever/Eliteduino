
#include "Matrix.h"

#include <LedControl.h>

namespace
{
	// We assume there is only one Max chip, so the address is always 0
	const uint8_t ADDRESS = 0;
}

void Eliteduino::Leds::Matrix::Initialize( uint8_t pinDIN, uint8_t pinCLK, uint8_t pinCS, uint8_t numRows, uint8_t numColumns, PCCommunications* comms )
{
	// A single driver can control 64 LEDs
	const uint8_t numberOfLedDriverChips = 1;
	m_ledController = new LedControl( pinDIN, pinCLK, pinCS, numberOfLedDriverChips );

	// The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
	m_ledController->shutdown( ADDRESS, false );
	
	// Set the brightness to a medium values
	m_ledController->setIntensity( ADDRESS, 15 );
	
	// and clear the display
	m_ledController->clearDisplay( ADDRESS );

	m_rowCount = numRows;
	m_columnCount = numColumns;
}

void Eliteduino::Leds::Matrix::Set( uint8_t row, uint8_t column, bool on )
{
	m_ledController->setLed( ADDRESS, row, column, on );
}

void Eliteduino::Leds::Matrix::SetAll( bool on )
{
	for ( int column = 0; column < m_columnCount; ++column )
	{
		for ( int row = 0; row < m_rowCount; ++row )
		{
			m_ledController->setLed( ADDRESS, row, column, on );
		}
	}
}
