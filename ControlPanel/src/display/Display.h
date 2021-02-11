#ifndef __ELITEDUINO_DISPLAY_H__
#define __ELITEDUINO_DISPLAY_H__

namespace Eliteduino
{
	namespace Display
	{
		class Display
		{
		public:
			void Setup();

			void Update();
		};

		extern Display gDisplay;
	}
}

#endif // __ELITEDUINO_DISPLAY_H__
