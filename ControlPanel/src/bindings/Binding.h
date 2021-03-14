#ifndef __ELITEDUINO_PC_BINDINGS_BINDING_H
#define __ELITEDUINO_PC_BINDINGS_BINDING_H

#include "../PCCommunicationsDefines.h"

namespace Eliteduino
{
	namespace Bindings
	{
		union Binding
		{
			struct
			{
				eVirtualControlType VirtualType;
				ePhysicalControlType PhysicalType;
				eControlRole ControlRole;
				uint8_t Value;
			};

			uint8_t Raw[ 4 ];
		};
	}
}

#endif // __ELITEDUINO_PC_BINDINGS_BINDING_H
