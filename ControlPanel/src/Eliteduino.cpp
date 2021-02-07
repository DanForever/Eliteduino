#include "Eliteduino.h"

#include <SPI.h>
#include <ILI9341_t3.h>
#include <font_Arial.h>

// For the Adafruit shield, these are the default.
#define TFT_DC  9
#define TFT_CS 10

ILI9341_t3* Eliteduino::Screen::m_tft = nullptr;

Eliteduino::Screen::Screen()
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

void Eliteduino::Screen::Clear()
{
	m_tft->fillScreen( ILI9341_BLACK );
}

void Eliteduino::Screen::PrintLine( const char* text )
{
	m_tft->println( text );
}

Eliteduino::GeneralInfo::GeneralInfo( Database* database )
	: m_database(database)
{
	m_database->Commander.Changed += [this]
	{
		DrawCommanderName();
	};

	m_database->System.Changed += [this]
	{
		DrawSystem();
	};

	m_database->Station.Changed += [this]
	{
		DrawStation();
	};

	m_database->StationType.Changed += [this]
	{
		DrawStation();
	};
}

void Eliteduino::Property<const char*>::Set( const char* text )
{
	strncpy( m_text, text, 64 );
	Changed();
}

#include "assets/commander.c"
#include "assets/coriolis.c"
#include "assets/ocellus.c"
#include "assets/orbis.c"
#include "assets/outpost.c"
#include "assets/megaship.c"
#include "assets/asteroidstation.c"
#include "assets/surfaceport2.c"
#include "assets/system.c"

namespace
{
	struct ImageData
	{
		const uint16_t width;
		const uint16_t height;
		const uint16_t* pixels;
	};

	void DrawData( ILI9341_t3* tft, const ImageData& image, const char* text, int16_t y )
	{
		const int16_t areaHeight = 32;
		int16_t areaWidth = tft->width();

		// First we need to blank the bit of screen we're drawing to
		tft->fillRect( 0, y, areaWidth, areaHeight, ILI9341_BLACK );

		// Now draw the specified image
		int16_t imageY = y + ( ( 32 - image.height ) / 2 );

		if ( image.pixels )
			tft->writeRect( 0, imageY, image.width, image.height, image.pixels );

		// And finally, the text
		tft->setTextSize( 5 );
		tft->setFont( Arial_20 );
		tft->setTextColor( ILI9341_WHITE );
		uint16_t textHeight = tft->measureTextHeight( text );

		int16_t textY = y + ( ( 32 - textHeight ) / 2 );
		tft->setCursor( image.width + 3, textY );
		tft->println( text );

		Serial.print( "Printing " );
		Serial.print( text );
		Serial.print( " at y = " );
		Serial.println( textY );
	}

	int16_t CalculateYOffset( int16_t index )
	{
		return ( 32 + 3 ) * index;
	}

	ImageData MapStationTypeToImage( Eliteduino::eStationType stationType)
	{
		switch ( stationType )
		{
		case Eliteduino::eStationType::Coriolis:
				return { (uint16_t)station_coriolis.width, (uint16_t)station_coriolis.height, reinterpret_cast<const uint16_t*>(station_coriolis.pixel_data) };

			case Eliteduino::eStationType::Ocellus:
				return { (uint16_t)station_ocellus.width, (uint16_t)station_ocellus.height, reinterpret_cast<const uint16_t*>( station_ocellus.pixel_data ) };

			case Eliteduino::eStationType::Orbis:
				return { (uint16_t)station_orbis.width, (uint16_t)station_orbis.height, reinterpret_cast<const uint16_t*>( station_orbis.pixel_data ) };

			case Eliteduino::eStationType::Outpost:
				return { (uint16_t)station_outpost.width, (uint16_t)station_outpost.height, reinterpret_cast<const uint16_t*>( station_outpost.pixel_data ) };

			case Eliteduino::eStationType::Megaship:
				return { (uint16_t)station_megaship.width, (uint16_t)station_megaship.height, reinterpret_cast<const uint16_t*>( station_megaship.pixel_data ) };

			case Eliteduino::eStationType::Asteroid:
				return { (uint16_t)station_asteroid.width, (uint16_t)station_asteroid.height, reinterpret_cast<const uint16_t*>( station_asteroid.pixel_data ) };

			case Eliteduino::eStationType::SurfacePort:
				return { (uint16_t)station_surfaceport.width, (uint16_t)station_surfaceport.height, reinterpret_cast<const uint16_t*>( station_surfaceport.pixel_data ) };

			case Eliteduino::eStationType::Unknown:
			default:
				return { 32, 32, nullptr };
		}
	}
}

void Eliteduino::GeneralInfo::DrawCommanderName()
{
	ImageData image = { (uint16_t)commander.width, (uint16_t)commander.height,reinterpret_cast<const uint16_t*>( commander.pixel_data ) };
	DrawData( m_tft, image, *m_database->Commander, CalculateYOffset( 0 ) );
}

void Eliteduino::GeneralInfo::DrawSystem()
{
	ImageData image = { (uint16_t)image_system.width, (uint16_t)image_system.height,reinterpret_cast<const uint16_t*>( image_system.pixel_data ) };
	DrawData( m_tft, image, *m_database->System, CalculateYOffset( 1 ) );
}

void Eliteduino::GeneralInfo::DrawStation()
{
	DrawData( m_tft, MapStationTypeToImage(*m_database->StationType), *m_database->Station, CalculateYOffset( 2 ) );
}
