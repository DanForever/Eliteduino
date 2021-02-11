#ifndef __ELITEDUINO_DISPLAY_SCREEN_GENERAL_H__
#define __ELITEDUINO_DISPLAY_SCREEN_GENERAL_H__

#include "Screen.h"

namespace Eliteduino
{
	namespace Display
	{
		class Database;

		class GeneralInfo final : public Screen
		{
		public:
			GeneralInfo( Database* database );

			void DrawCommanderName();
			void DrawSystem();
			void DrawStation();

		private:
			Database* m_database;
		};
	}
}

#endif // __ELITEDUINO_DISPLAY_SCREEN_GENERAL_H__
