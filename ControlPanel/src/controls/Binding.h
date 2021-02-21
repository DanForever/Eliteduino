#ifndef __ELITEDUINO_PC_BUTTON_BINDING_H
#define __ELITEDUINO_PC_BUTTON_BINDING_H

#include "../PCCommunicationsDefines.h"

namespace Eliteduino
{
	namespace Controls
	{
		union Binding
		{
			struct
			{
				eVirtualControlType VirtualType;
				ePhysicalControlType PhysicalType;
				uint8_t Value;
			};

			uint8_t Raw[ 3 ];
		};
	}
}

#endif // __ELITEDUINO_PC_BUTTON_BINDING_H
