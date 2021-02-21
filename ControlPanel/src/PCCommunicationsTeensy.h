#ifndef __ELITEDUINO_PC_COMMUNICATIONS_TEENSY_H__
#define __ELITEDUINO_PC_COMMUNICATIONS_TEENSY_H__

#define DO_NOT_WAIT 0

#include <Arduino.h>

namespace Eliteduino
{
	namespace Hid
	{
		inline void Initialize() {}

		inline uint8_t Recieve( uint8_t* buffer ) { return RawHID.recv( buffer, DO_NOT_WAIT ); }

		void SendInputEvent( eVirtualControlType controlType, eEventType eventType, uint8_t controlId ) {}
	}
}

#endif // __ELITEDUINO_PC_COMMUNICATIONS_TEENSY_H__
