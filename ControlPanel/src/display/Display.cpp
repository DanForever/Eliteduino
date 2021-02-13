#include "../Devices.h"

#ifdef ELITEDUINO_SCREEN

#include "Display.h"

#include "ScreenGeneral.h"
#include "Database.h"
#include "../PCCommunications.h"

#include <Arduino.h>

namespace
{
	enum StatType
	{
		Cmdr = 1,
		System,
		Station,
		StationType,
	};

	void ProcessStringStat( const uint8_t* sourceBuffer, Eliteduino::Property<const char*>& targetProperty )
	{
		targetProperty = reinterpret_cast<const char*>( sourceBuffer );
	}
}

Eliteduino::Display::Display Eliteduino::Display::gDisplay;

void Eliteduino::Display::Display::Setup()
{
	m_database = new Eliteduino::Display::Database();
	m_screen = new Eliteduino::Display::GeneralInfo( m_database );
}

void Eliteduino::Display::Display::Update()
{
}

void Eliteduino::Display::Display::ProcessMessage( const Message& message )
{
	switch ( message.Data.Type )
	{
	case MessageType::Data:
		ProcessStatReport( message );
		break;

	default:
		break;
	}
}

void Eliteduino::Display::Display::ProcessStatReport( const Message& message )
{
	constexpr uint8_t StatTypeBufferPosition = 0;
	constexpr uint8_t DataBufferPosition = 1;

	StatType statType = static_cast<StatType>( message.Data.Buffer[ StatTypeBufferPosition ] );
	const uint8_t* data = message.Data.Buffer + DataBufferPosition;

	switch ( statType )
	{
	case StatType::Cmdr:
		ProcessStringStat( data, m_database->Commander);

		Serial.print( "Commander name received: " );
		Serial.println( *m_database->Commander );
		break;

	case StatType::System:
		ProcessStringStat( data, m_database->System );

		Serial.print( "System received: " );
		Serial.println( *m_database->System );
		break;

	case StatType::Station:
		ProcessStringStat( data, m_database->Station );

		Serial.print( "Station received: " );
		Serial.println( *m_database->Station );
		break;

	case StatType::StationType:
		m_database->StationType = static_cast<Eliteduino::Display::eStationType>( *data );

		Serial.print( "Station type received: " );
		Serial.println( *m_database->StationType );
		break;

	default:
		Serial.print( "Unrecognised stat type received: " );
		Serial.println( (int)statType );
	}
}

#endif // ELITEDUINO_SCREEN