#ifndef __ELITEDUINO_PROPERTY_H__
#define __ELITEDUINO_PROPERTY_H__

#include "Event.h"

namespace Eliteduino
{
	//---------------------------------------------------------------------------------------------------------------------------------
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

		Event Changed;

	private:
		T m_value = {};
	};

	//---------------------------------------------------------------------------------------------------------------------------------
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

		Event Changed;

	private:
		char m_text[ 64 ];
	};
}

#endif // __ELITEDUINO_PROPERTY_H__
