#ifndef _ELITEDUINO_H_
#define _ELITEDUINO_H_

#include <functional>
#include <vector>
#include <utility>

class ILI9341_t3;

namespace Eliteduino
{
	template< typename... Args >
	class Event
	{
	public:
		using Callback = std::function<void( Args... )>;

		void operator+=( Callback&& callback ) { m_listeners.push_back( std::move(callback) ); }
		void operator()(Args... args)
		{
			for ( Callback& callback : m_listeners )
			{
				callback( std::forward<Args>(args)... );
			}
		}

	private:
		std::vector<Callback> m_listeners;
	};

	class Screen
	{
	public:
		Screen();

		void Clear();
		void PrintLine( const char* text );

	protected:
		static ILI9341_t3* m_tft;
	};

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

	template <typename T>
	class Property
	{
	public:
		void Set( const T& value )
		{
			m_value = value;
			Changed();
		}

		const T& Get() const
		{
			return m_value;
		}

		void operator=( const T& value )
		{
			Set( value );
		}

		const T& operator*() const
		{
			return Get();
		}

		Event<> Changed;

	private:
		T m_value = {};
	};

	template <>
	class Property<const char*>
	{
	public:
		void Set( const char* text );

		const char* Get() const
		{
			return m_text;
		}

		void operator=( const char* text )
		{
			Set( text );
		}

		const char* operator*() const
		{
			return Get();
		}

		Event<> Changed;

	private:
		char m_text[ 64 ];
	};

	class Database
	{
	public:

		Property<const char*> Commander;
		Property<const char*> System;
		Property<const char*> Station;
		Property<eStationType> StationType;
	};

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

#endif // _ELITEDUINO_H_
