#ifndef __ELITEDUINO_PC_COMMUNICATIONS_H__
#define __ELITEDUINO_PC_COMMUNICATIONS_H__

#include <stdint.h>

namespace Eliteduino
{
	enum MessageType
	{
		Data = 1,
		ControlsRebind,
	};

	struct MessageData
	{
		MessageType Type;
		uint8_t Buffer[ 63 ];
	};

	struct Message
	{
		uint8_t Size;
		MessageData Data;
	};

	class PCCommunications
	{
	public:
		void Update();

		bool MessageReceived() const { return m_message.Size > 0; }
		const Message& GetMessage() const { return m_message; }

	private:
		Message m_message;
		bool m_messageReceieved = false;
	};
}

#endif // __ELITEDUINO_PC_COMMUNICATIONS_H__
