#ifndef __ELITEDUINO_DISPLAY_SCREEN_H__
#define __ELITEDUINO_DISPLAY_SCREEN_H__

class ILI9341_t3;

namespace Eliteduino
{
	namespace Display
	{
		class Screen
		{
		public:
			Screen();

			void Clear();
			void PrintLine( const char* text );

		protected:
			static ILI9341_t3* m_tft;
		};
	}
}

#endif // __ELITEDUINO_DISPLAY_SCREEN_H__
