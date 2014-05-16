#if !defined _IRC_SERVER_H_HEADER
#define _IRC_SERVER_H_HEADER

#include "Thinker.h"
#include "TCPSocket.h"
#include "INIFile.h"
#include "IRC.h"

namespace Jupiter
{

	namespace IRC
	{

		class Server : public Thinker
		{
			struct Data;
			Data *data_;
			class Client
			{
			};
			class Channel
			{
			};

		};

	}

}

#endif // _IRC_SERVER_H_HEADER