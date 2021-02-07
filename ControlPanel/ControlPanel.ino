#include "src/Eliteduino.h"

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

Eliteduino::Database* database = nullptr;
Eliteduino::Screen* screen = nullptr;

void setup()
{
	Serial.begin( 9600 );
	Serial.println( F( "RawHID Example" ) );
	for ( int i = 0; i < 7; i++ ) {
		pinMode( i, OUTPUT );
	}

	pinMode( 17, INPUT_PULLUP );

	database = new Eliteduino::Database();
	screen = new Eliteduino::GeneralInfo( database );
}

// RawHID packets are always 64 bytes
byte buffer[ 64 ];
elapsedMillis msUntilNextSend;
unsigned int packetCount = 0;

int previousButtonState = HIGH;

void TestCheckButton()
{
	int buttonState = digitalRead( 17 );

	if ( buttonState != previousButtonState )
	{
		if ( buttonState == HIGH )
		{
			Serial.println( "Button Released" );
		}
		else
			Serial.println( "Button Pressed" );

		previousButtonState = buttonState;
	}
}

void ProcessCommander( byte* buffer, int size )
{
	const char* textInBuffer = reinterpret_cast<const char*>( buffer + StatDataStartingByte );

	database->Commander = textInBuffer;

	Serial.print( "Commander name received: " );
	Serial.println( *database->Commander );
}

void ProcessSystem( byte* buffer, int size )
{
	const char* textInBuffer = reinterpret_cast<const char*>( buffer + StatDataStartingByte );

	database->System = textInBuffer;

	Serial.print( "System received: " );
	Serial.println( *database->System );
}

void ProcessStation( byte* buffer, int size )
{
	const char* textInBuffer = reinterpret_cast<const char*>( buffer + StatDataStartingByte );

	database->Station = textInBuffer;

	Serial.print( "Station received: " );
	Serial.println( *database->Station );
}

void ProcessStationType( byte* buffer, int size )
{
	database->StationType = static_cast<Eliteduino::eStationType>( *( buffer + StatDataStartingByte ) );

	Serial.print( "Station type received: " );
	Serial.println( *database->StationType );
}

void ProcessStatReport( byte* buffer, int size )
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

		// Dummy code to light up leds based on contents of hid report
		for ( int i = 0; i < 8; i++ ) {
			int b = buffer[ 1 ] & ( 1 << i );
			digitalWrite( i, b );
		}
	}
}

void loop()
{
	TestCheckButton();
	CheckForIncomingHidReports();

	/*
	// every 2 seconds, send a packet to the computer
	if ( msUntilNextSend > 2000 ) {
		msUntilNextSend = msUntilNextSend - 2000;
		// first 2 bytes are a signature
		buffer[ 0 ] = 0xAB;
		buffer[ 1 ] = 0xCD;
		// next 24 bytes are analog measurements
		for ( int i = 0; i < 12; i++ ) {
			int val = analogRead( i );
			buffer[ i * 2 + 2 ] = highByte( val );
			buffer[ i * 2 + 3 ] = lowByte( val );
		}
		// fill the rest with zeros
		for ( int i = 26; i < 62; i++ ) {
			buffer[ i ] = 0;
		}
		// and put a count of packets sent at the end
		buffer[ 62 ] = highByte( packetCount );
		buffer[ 63 ] = lowByte( packetCount );
		// actually send the packet
		n = RawHID.send( buffer, 100 );
		if ( n > 0 ) {
			Serial.print( F( "Transmit packet " ) );
			Serial.println( packetCount );
			packetCount = packetCount + 1;
		}
		else {
			Serial.println( F( "Unable to transmit packet" ) );
		}
	}//*/
}
