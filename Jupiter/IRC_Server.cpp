/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include "IRC_Server.h"
#include <string>

struct SClient
{
	Jupiter::Socket *sock;
};

struct Jupiter::IRC::Server::Data
{
	Data();
	~Data();
};