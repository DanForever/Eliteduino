#include "src/Eliteduino.h"

Eliteduino::PCCommunications Comms;

void setup()
{
	Serial.begin( 9600 );

	Comms.Initialize();

	DISPLAY_SETUP();
	BUTTONS_SETUP(&Comms);
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
	BUTTONS_UPDATE();
}
