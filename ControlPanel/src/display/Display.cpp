#include "Display.h"

#include "ScreenGeneral.h"
#include "Database.h"

#include <Arduino.h>

namespace
{
	constexpr int ReportTypeBytePosition = 0;
	constexpr int StatTypeBytePosition = 1;
	constexpr int StatDataStartingByte = 2;

	enum ReportType
	{
		Data = 1,
		ControlsRebind,
	};

	enum StatType
	{
		Cmdr = 1,
		System,
		Station,
		StationType,
	};

	Eliteduino::Display::Database* database = nullptr;
	Eliteduino::Display::Screen* screen = nullptr;

	void screen_setup()
	{
		database = new Eliteduino::Display::Database();
		screen = new Eliteduino::Display::GeneralInfo( database );
	}

	// RawHID packets are always 64 bytes
	uint8_t buffer[ 64 ];
	unsigned int packetCount = 0;

	void ProcessCommander( uint8_t* buffer, int size )
	{
		const char* textInBuffer = reinterpret_cast<const char*>( buffer + StatDataStartingByte );

		database->Commander = textInBuffer;

		Serial.print( "Commander name received: " );
		Serial.println( *database->Commander );
	}

	void ProcessSystem( uint8_t* buffer, int size )
	{
		const char* textInBuffer = reinterpret_cast<const char*>( buffer + StatDataStartingByte );

		database->System = textInBuffer;

		Serial.print( "System received: " );
		Serial.println( *database->System );
	}

	void ProcessStation( uint8_t* buffer, int size )
	{
		const char* textInBuffer = reinterpret_cast<const char*>( buffer + StatDataStartingByte );

		database->Station = textInBuffer;

		Serial.print( "Station received: " );
		Serial.println( *database->Station );
	}

	void ProcessStationType( uint8_t* buffer, int size )
	{
		database->StationType = static_cast<Eliteduino::Display::eStationType>( *( buffer + StatDataStartingByte ) );

		Serial.print( "Station type received: " );
		Serial.println( *database->StationType );
	}

	void ProcessStatReport( uint8_t* buffer, int size )
	{
		StatType statType = static_cast<StatType>( buffer[ StatTypeBytePosition ] );

		switch ( statType )
		{
		case StatType::Cmdr:
			Serial.println( "Commander name received!" );
			ProcessCommander( buffer, size );
			break;

		case StatType::System:
			Serial.println( "System received!" );
			ProcessSystem( buffer, size );
			break;

		case StatType::Station:
			Serial.println( "Station received!" );
			ProcessStation( buffer, size );
			break;

		case StatType::StationType:
			Serial.println( "Station type received!" );
			ProcessStationType( buffer, size );
			break;

		default:
			Serial.print( "Unrecognised stat type received: " );
			Serial.println( (int)statType );
		}
	}

	void CheckForIncomingHidReports()
	{
		int bytesReceived;
		bytesReceived = RawHID.recv( buffer, 0 ); // 0 timeout = do not wait
		if ( bytesReceived > 0 )
		{
			ReportType reportType = static_cast<ReportType>( buffer[ ReportTypeBytePosition ] );

			Serial.print( "Report received, length: " );
			Serial.println( bytesReceived );

			for ( int i = 0; i < bytesReceived; ++i )
			{
				Serial.print( buffer[ i ], 16 );
				Serial.print( " " );
			}

			Serial.println( "" );

			switch ( reportType )
			{
			case ReportType::Data:
				Serial.println( "Stat recieved!" );
				ProcessStatReport( buffer, bytesReceived );
				break;

			default:
				Serial.print( "Unrecognised report type received: " );
				Serial.println( (int)reportType );
			}
		}
	}

	void screen_loop()
	{
		CheckForIncomingHidReports();
	}
}

Eliteduino::Display::Display Eliteduino::Display::gDisplay;

void Eliteduino::Display::Display::Setup()
{
	screen_setup();
}

void Eliteduino::Display::Display::Update()
{
	screen_loop();
}
