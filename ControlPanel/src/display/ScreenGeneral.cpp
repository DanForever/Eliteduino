#include "../Devices.h"

#ifdef ELITEDUINO_SCREEN

#include "ScreenGeneral.h"

#include "Assets.h"
#include "Database.h"

#include <font_Arial.h>
#include <ILI9341_t3.h>


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

	ImageData MapStationTypeToImage( Eliteduino::Display::eStationType stationType )
	{
		using namespace Eliteduino::Display;

		switch ( stationType )
		{
		case eStationType::Coriolis:
			return { (uint16_t)station_coriolis.width, (uint16_t)station_coriolis.height, reinterpret_cast<const uint16_t*>( station_coriolis.pixel_data ) };

		case eStationType::Ocellus:
			return { (uint16_t)station_ocellus.width, (uint16_t)station_ocellus.height, reinterpret_cast<const uint16_t*>( station_ocellus.pixel_data ) };

		case eStationType::Orbis:
			return { (uint16_t)station_orbis.width, (uint16_t)station_orbis.height, reinterpret_cast<const uint16_t*>( station_orbis.pixel_data ) };

		case eStationType::Outpost:
			return { (uint16_t)station_outpost.width, (uint16_t)station_outpost.height, reinterpret_cast<const uint16_t*>( station_outpost.pixel_data ) };

		case eStationType::Megaship:
			return { (uint16_t)station_megaship.width, (uint16_t)station_megaship.height, reinterpret_cast<const uint16_t*>( station_megaship.pixel_data ) };

		case eStationType::Asteroid:
			return { (uint16_t)station_asteroid.width, (uint16_t)station_asteroid.height, reinterpret_cast<const uint16_t*>( station_asteroid.pixel_data ) };

		case eStationType::SurfacePort:
			return { (uint16_t)station_surfaceport.width, (uint16_t)station_surfaceport.height, reinterpret_cast<const uint16_t*>( station_surfaceport.pixel_data ) };

		case eStationType::Unknown:
		default:
			return { 32, 32, nullptr };
		}
	}
}

Eliteduino::Display::GeneralInfo::GeneralInfo( Database* database )
	: m_database( database )
{
	m_database->Commander.Changed += new Listener<GeneralInfo>( this, &GeneralInfo::DrawCommanderName );
	m_database->System.Changed += new Listener<GeneralInfo>( this, &GeneralInfo::DrawSystem );
	m_database->Station.Changed += new Listener<GeneralInfo>( this, &GeneralInfo::DrawStation );
	m_database->StationType.Changed += new Listener<GeneralInfo>( this, &GeneralInfo::DrawStation );
}

void Eliteduino::Display::GeneralInfo::DrawCommanderName()
{
	ImageData image = { (uint16_t)commander.width, (uint16_t)commander.height,reinterpret_cast<const uint16_t*>( commander.pixel_data ) };
	DrawData( m_tft, image, *m_database->Commander, CalculateYOffset( 0 ) );
}

void Eliteduino::Display::GeneralInfo::DrawSystem()
{
	ImageData image = { (uint16_t)image_system.width, (uint16_t)image_system.height,reinterpret_cast<const uint16_t*>( image_system.pixel_data ) };
	DrawData( m_tft, image, *m_database->System, CalculateYOffset( 1 ) );
}

void Eliteduino::Display::GeneralInfo::DrawStation()
{
	DrawData( m_tft, MapStationTypeToImage( *m_database->StationType ), *m_database->Station, CalculateYOffset( 2 ) );
}

#endif // ELITEDUINO_SCREEN
