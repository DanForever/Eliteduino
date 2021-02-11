#include "src/Eliteduino.h"

void setup()
{
	Serial.begin( 9600 );

	DISPLAY_SETUP();
}

void loop()
{
	DISPLAY_UPDATE();
}
