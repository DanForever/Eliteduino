#ifndef __ELITEDUINO_PC_COMMUNICATIONS_SERIAL_H__
#define __ELITEDUINO_PC_COMMUNICATIONS_SERIAL_H__

#include "Architecture.h"
#include "PCCommunicationsDefines.h"

namespace Eliteduino
{
	// Unfortunately, using serial communications will make debugging hard, since serial is normally available for sending debug text back to the PC
	namespace Serial
	{
		void Initialize();

		uint8_t Recieve( uint8_t* buffer );
	}

#ifdef ELITEDUINO_PC_COMMS_SERIAL
	namespace Comms = Serial;
#endif
}

#endif // __ELITEDUINO_PC_COMMUNICATIONS_SERIAL_H__
