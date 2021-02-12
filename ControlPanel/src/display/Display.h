#ifndef __ELITEDUINO_DISPLAY_H__
#define __ELITEDUINO_DISPLAY_H__

namespace Eliteduino
{
	struct Message;

	namespace Display
	{
		class Database;
		class Screen;

		class Display
		{
		public:
			void Setup();
			void Update();

			void ProcessMessage( const Message& message );

		private:
			void ProcessStatReport( const Message& message );

			Database* m_database = nullptr;
			Screen* m_screen = nullptr;
		};

		extern Display gDisplay;
	}
}

#endif // __ELITEDUINO_DISPLAY_H__
