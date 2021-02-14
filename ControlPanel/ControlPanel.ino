#include "src/Eliteduino.h"

Eliteduino::PCCommunications Comms;

void setup()
{
	Serial.begin( 9600 );

	Comms.Initialize();

	DISPLAY_SETUP();
}

void loop()
{
	Comms.Update();

	if ( Comms.MessageReceived() )
	{
		const Eliteduino::Message& message = Comms.GetMessage();

		DISPLAY_PROCESS_MESSAGE( message );
	}

	DISPLAY_UPDATE();
}
