#include "Property.h"

#include <string.h>

void Eliteduino::Property<const char*>::Set( const char* text )
{
	strncpy( m_text, text, 64 );
	Changed();
}
