#include "PCCommunications.h"

#include <Arduino.h>
#include "debug/Debug.h"
#include "PCCommunicationsSerial.h"

#ifdef ELITEDUINO_TEENSY
#	include "PCCommunicationsTeensy.h"
#else
#	include "PCCommunicationsArduino.h"
#endif


void Eliteduino::PCCommunications::Initialize()
{
	Serial::Initialize();
	Hid::Initialize();
}

void Eliteduino::PCCommunications::Update()
{
	m_message.Size = Eliteduino::Comms::Recieve( reinterpret_cast<uint8_t*>( &m_message.Data ) );
}

void Eliteduino::PCCommunications::SendInputEvent( Eliteduino::eVirtualControlType controlType, Eliteduino::eEventType eventType, ControlCode controlId )
{
	Hid::SendInputEvent( controlType, eventType, controlId );
}
