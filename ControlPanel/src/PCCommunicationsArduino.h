#ifndef __ELITEDUINO_PC_COMMUNICATIONS_ARDUINO_H__
#define __ELITEDUINO_PC_COMMUNICATIONS_ARDUINO_H__

#include "Architecture.h"
#include "PCCommunicationsDefines.h"

namespace Eliteduino
{
	namespace Hid
	{
		void Initialize();

		uint8_t Recieve( uint8_t* buffer );

		void SendInputEvent( eVirtualControlType controlType, eEventType eventType, ControlCode controlId );
	}

#ifdef ELITEDUINO_PC_COMMS_HID
	namespace Comms = Hid;
#endif

}

#endif // __ELITEDUINO_PC_COMMUNICATIONS_ARDUINO_H__
