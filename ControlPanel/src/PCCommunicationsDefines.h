#ifndef __ELITEDUINO_PC_COMMUNICATIONS_DEFINES_H__
#define __ELITEDUINO_PC_COMMUNICATIONS_DEFINES_H__

#include <stdint.h>

namespace Eliteduino
{
	enum class eVirtualControlType : uint8_t
	{
		Keyboard,
		Gamepad
	};

	enum class ePhysicalControlType : uint8_t
	{
		// Sends a "pressed" event when the user presses the button down, or toggles the switch on
		// Similarly sends a "released" event when the user releases the button, or toggles the switch off
		// Most buttons are considered momentary
		Momentary,

		// Sends a "pressed" followed by an immediate "release" event each time the user presses or releases (toggles on or off) a button
		// This is useful for phyiscal toggle/rocker switches that want to send the same command the button state changes, regardless of physical button state
		Toggle,
	};

	enum class eEventType : uint8_t
	{
		Press,
		Release
	};

	enum class eControlRole : uint8_t
	{
		Undefined = 255,

		LandingGear = 0,
		CargoScoop,
		Lights,
		SilentRunning,
		FlightAssist,
		RotationalAssist,
		GalaxyMap,
		SystemMap,
		FrameshiftJump,
		Supercruise,
		Heatsink,

		Count
	};

	enum class StatType : uint8_t
	{
		Cmdr = 1,
		System,
		Station,
		StationType,
		FsdCharging,
		FsdCooldown,
		MassLocked,
		Overheating,
		BeingInterdicted,
		HardpointsDeployed,
	};
}

#endif // __ELITEDUINO_PC_COMMUNICATIONS_DEFINES_H__
