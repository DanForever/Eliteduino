#ifndef __ELITEDUINO_EVENT_H__
#define __ELITEDUINO_EVENT_H__

#include "Containers.h"

namespace Eliteduino
{
	//--------------------------------------------------------------------------------------------------------
	class ListenerBase
	{
	public:
		virtual void Invoke() = 0;
	};

	//--------------------------------------------------------------------------------------------------------
	template <typename TSelf>
	class Listener : public ListenerBase
	{
	public:
		using Callback = void ( TSelf::* )();

		Listener( TSelf* self, Callback callback )
			: m_self( self )
			, m_callback(callback)
		{}

		void Invoke() final
		{
			( m_self->*m_callback )();
		}

	private:
		TSelf* m_self;
		Callback m_callback;
	};

	//--------------------------------------------------------------------------------------------------------
	class Event
	{
	public:
		void operator+=( ListenerBase* listener ) { m_listeners.push_back( listener ); }
		void operator()()
		{
			for ( ListenerBase*& listener : m_listeners )
			{
				listener->Invoke();
			}
		}

	private:
		Containers::Vector<ListenerBase*> m_listeners;
	};
}

#endif // __ELITEDUINO_EVENT_H__
