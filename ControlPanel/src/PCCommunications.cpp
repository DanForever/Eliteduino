#include "PCCommunications.h"

#include <Arduino.h>

#define DO_NOT_WAIT 0

void Eliteduino::PCCommunications::Update()
{
	m_message.Size = RawHID.recv( &m_message.Data, DO_NOT_WAIT );
}
