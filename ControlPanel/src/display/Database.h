#ifndef __ELITEDUINO_DISPLAY_DATABASE_H__
#define __ELITEDUINO_DISPLAY_DATABASE_H__

#include "../Property.h"

namespace Eliteduino
{
	namespace Display
	{
		enum eStationType
		{
			Unknown = -1,
			Coriolis = 1,
			Ocellus,
			Orbis,
			Outpost,
			Megaship,
			Asteroid,
			SurfacePort,
		};

		class Database
		{
		public:

			Property<const char*> Commander;
			Property<const char*> System;
			Property<const char*> Station;
			Property<eStationType> StationType;
		};
	}
}

#endif // __ELITEDUINO_DISPLAY_DATABASE_H__
