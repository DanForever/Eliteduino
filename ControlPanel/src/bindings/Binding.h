#ifndef __ELITEDUINO_PC_BINDINGS_BINDING_H
#define __ELITEDUINO_PC_BINDINGS_BINDING_H

#include "../PCCommunicationsDefines.h"

namespace Eliteduino
{
	namespace Bindings
	{
		struct Binding
		{
			eVirtualControlType VirtualType = eVirtualControlType::Gamepad;
			ePhysicalControlType PhysicalType = ePhysicalControlType::Momentary;
			eControlRole ControlRole = eControlRole::Undefined;
			uint8_t Value = 0;

			uint8_t* Raw() { return reinterpret_cast<uint8_t*>( this ); }
			const uint8_t* Raw() const { return reinterpret_cast<const uint8_t*>( this ); }
			uint8_t Raw( uint8_t index ) const { return Raw()[ index ]; }
			uint8_t& Raw( uint8_t index ) { return Raw()[ index ]; }
		};
	}
}

#endif // __ELITEDUINO_PC_BINDINGS_BINDING_H
