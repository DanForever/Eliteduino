#ifndef __ELITEDUINO_DEBUG_H__
#define __ELITEDUINO_DEBUG_H__

#include <Arduino.h>

#define ELITEDUINO_DEBUG
#define ELITEDUINO_WAIT_FOR_SERIAL

#ifdef ELITEDUINO_DEBUG

#ifdef ELITEDUINO_WAIT_FOR_SERIAL
#	define SERIAL_CHECK while ( !Serial );
#else
#	define SERIAL_CHECK if ( Serial )
#endif // ELITEDUINO_WAIT_FOR_SERIAL

namespace Debug
{
	inline void Print()
	{
		Serial.println();
	}
	
	template <typename Arg, typename... Args>
	inline void Print( Arg&& arg, Args&&... args )
	{
		Serial.print( arg );
		Print( args... );
	}
}

#	define PRINT(...) SERIAL_CHECK Debug::Print(__VA_ARGS__)
#else
#	define PRINT(...)
#endif // ELITEDUINO_DEBUG

#endif // __ELITEDUINO_DEBUG_H__
