#ifndef _ELITEDUINO_H_
#define _ELITEDUINO_H_

#include "Devices.h"

#ifdef ELITEDUINO_SCREEN

#include "display/Display.h"

#define DISPLAY_SETUP() Eliteduino::Display::gDisplay.Setup()
#define DISPLAY_UPDATE() Eliteduino::Display::gDisplay.Update()

#else

#define DISPLAY_SETUP(...)
#define DISPLAY_UPDATE(...)

#endif // ELITEDUINO_SCREEN

namespace Eliteduino
{
}

#endif // _ELITEDUINO_H_
