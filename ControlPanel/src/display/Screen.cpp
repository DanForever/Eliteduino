#include "../Devices.h"

#ifdef ELITEDUINO_SCREEN

#include "Screen.h"

#include <SPI.h>
#include <ILI9341_t3.h>

// For the Adafruit shield, these are the default.
#define TFT_DC  9
#define TFT_CS 10

ILI9341_t3* Eliteduino::Display::Screen::m_tft = nullptr;

Eliteduino::Display::Screen::Screen()
{
	if ( !m_tft )
	{
		m_tft = new ILI9341_t3( TFT_CS, TFT_DC );
		m_tft->begin();
		m_tft->setRotation( 3 );
		m_tft->setTextWrap( false );
		m_tft->fillScreen( ILI9341_BLACK );
	}
}

void Eliteduino::Display::Screen::Clear()
{
	m_tft->fillScreen( ILI9341_BLACK );
}

void Eliteduino::Display::Screen::PrintLine( const char* text )
{
	m_tft->println( text );
}

#endif // ELITEDUINO_SCREEN
