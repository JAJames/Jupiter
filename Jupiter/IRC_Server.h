/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

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