#ifndef _ELITEDUINO_H_
#define _ELITEDUINO_H_

#include "Devices.h"
#include "PCCommunications.h"

#ifdef ELITEDUINO_SCREEN

#include "display/Display.h"

#define DISPLAY_SETUP() Eliteduino::Display::gDisplay.Setup()
#define DISPLAY_UPDATE() Eliteduino::Display::gDisplay.Update()
#define DISPLAY_PROCESS_MESSAGE( message ) Eliteduino::Display::gDisplay.ProcessMessage( message )

#else

#define DISPLAY_SETUP(...)
#define DISPLAY_UPDATE(...)
#define DISPLAY_PROCESS_MESSAGE(...)

#endif // ELITEDUINO_SCREEN

namespace Eliteduino
{
}

#endif // _ELITEDUINO_H_
