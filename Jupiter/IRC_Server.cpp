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