#ifndef __ELITEDUINO_EVENT_H__
#define __ELITEDUINO_EVENT_H__

#include <functional>
#include <vector>

namespace Eliteduino
{
	template< typename... Args >
	class Event
	{
	public:
		using Callback = std::function<void( Args... )>;

		void operator+=( Callback&& callback ) { m_listeners.push_back( std::move( callback ) ); }
		void operator()( Args... args )
		{
			for ( Callback& callback : m_listeners )
			{
				callback( std::forward<Args>( args )... );
			}
		}

	private:
		std::vector<Callback> m_listeners;
	};
}

#endif // __ELITEDUINO_EVENT_H__
