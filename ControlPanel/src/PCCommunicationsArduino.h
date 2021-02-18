#ifndef __ELITEDUINO_PC_COMMUNICATIONS_ARDUINO_H__
#define __ELITEDUINO_PC_COMMUNICATIONS_ARDUINO_H__

#include <stdint.h>

#include "PCCommunicationsDefines.h"

namespace Eliteduino
{
	namespace Hid
	{
		void Initialize();

		uint8_t Recieve( uint8_t* buffer );

		void SendInputEvent( eControlType controlType, eEventType eventType, uint8_t controlId );
	}
}

#endif // __ELITEDUINO_PC_COMMUNICATIONS_ARDUINO_H__
