#include "PCCommunications.h"

#include <stdint.h>

#include "Architecture.h"

#ifdef ELITEDUINO_TEENSY
#	include "PCCommunicationsTeensy.h"
#else
#	include "PCCommunicationsArduino.h"
#endif

#define DO_NOT_WAIT 0

void Eliteduino::PCCommunications::Initialize()
{
	Hid::Initialize();
}

void Eliteduino::PCCommunications::Update()
{
	m_message.Size = Hid::Recieve( reinterpret_cast<uint8_t*>( &m_message.Data ) );
}
