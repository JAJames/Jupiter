/**
 * Copyright (C) 2013-2016 Jessica James.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Written by Jessica James <jessica.aj@outlook.com>
 */

#include <cstring>
#include <cstdio>
#include <ctime>
#include "Jupiter.h"
#include "Functions.h"
#include "IRC_Client.h"
#include "INIFile.h"
#include "TCPSocket.h"
#include "CString.h"
#include "String.h"
#include "Plugin.h"
#include "Base64.h"

//#define SHORT_IRC_MACROS
#include "IRC_Numerics.h"

#if defined _WIN32
#include <WinSock2.h>
#else // _WIN32
#include <unistd.h>
#endif // _WIN32

using namespace Jupiter::literals;

Jupiter::INIFile _Config;
Jupiter::INIFile *Jupiter::IRC::Client::Config = &_Config;

template class JUPITER_API Jupiter::Reference_String<char>;
template class JUPITER_API Jupiter::String_Strict<char>;
template class JUPITER_API Jupiter::CString_Type<char>;
template class JUPITER_API Jupiter::ArrayList<Jupiter::IRC::Client::User>;
template class JUPITER_API Jupiter::ArrayList<Jupiter::IRC::Client::Channel>;

struct JUPITER_API Jupiter::IRC::Client::Data
{
	Jupiter::IRC::Client *iFace;
	Jupiter::Socket *sock;
	bool ssl;
	Jupiter::StringS SSLCertificate;
	Jupiter::StringS SSLKey;
	Jupiter::StringS saslAccount;
	Jupiter::StringS saslPass;
	int connectionStatus;
	Jupiter::StringS configSectionName;
	Jupiter::CStringS logFileName;
	Jupiter::ReferenceString buffer;
	unsigned short serverPort;
	Jupiter::CStringS serverHostname;
	Jupiter::StringS serverName;
	Jupiter::StringS nickname;
	Jupiter::StringS realName;
	Jupiter::StringS prefixModes = "ov";
	Jupiter::StringS prefixes = "@+";
	Jupiter::StringS chanTypes = "#";
	Jupiter::StringS modeA = "b";
	Jupiter::StringS modeB = "k";
	Jupiter::StringS modeC = "l";
	Jupiter::StringS modeD = "psitnm";
	Jupiter::ArrayList<Channel> channels;
	void delChannel(unsigned int index);
	void delChannel(const Jupiter::ReadableString &chan);
	void addNamesToChannel(unsigned int channelIndex, Jupiter::ReadableString &names);
	size_t addChannel(const Jupiter::ReadableString &chan);
	bool joinOnKick;
	Jupiter::StringS autoPartMessage;
	time_t reconnectDelay;
	time_t reconnectTime;
	int maxReconnectAttempts;
	int reconnectAttempts;
	FILE *printOutput;
	FILE *logFile;
	int dChanType;
	bool dead = false;
	Jupiter::ArrayList<Jupiter::IRC::Client::User> users;

	bool startCAP();
	bool registerClient();
	Data(Jupiter::IRC::Client *interFace);
	Jupiter::IRC::Client::User *findUser(const Jupiter::ReadableString &nick) const;
	Jupiter::IRC::Client::User *findUserOrAdd(const Jupiter::ReadableString &nick);
};

Jupiter::IRC::Client::Data::Data(Jupiter::IRC::Client *interFace)
{
	Jupiter::IRC::Client::Data::iFace = interFace;
}

struct Jupiter::IRC::Client::User::Data
{
	unsigned int channelCount = 0;
	Jupiter::StringS nickname;
	Jupiter::StringS username;
	Jupiter::StringS hostname;
};

struct Jupiter::IRC::Client::Channel::User::Data
{
	Jupiter::IRC::Client::User *user;
	Jupiter::StringS prefixes;

	~Data();
};

Jupiter::IRC::Client::Channel::User::Data::~Data()
{
	--Jupiter::IRC::Client::Channel::User::Data::user->data_->channelCount;
}

struct Jupiter::IRC::Client::Channel::Data
{
	Jupiter::StringS channel;
	Jupiter::ArrayList<Jupiter::IRC::Client::Channel::User> users;
	Jupiter::ReferenceString serverPrefixSetPtr;
	int type;
	bool isAddingNames;
};

Jupiter::IRC::Client::Client(const Jupiter::ReadableString &configSection)
{
	Jupiter::IRC::Client::data_ = new Jupiter::IRC::Client::Data(this);
	if (Jupiter::IRC::Client::Config == nullptr)
	{
		Jupiter::IRC::Client::Config = new INIFile();
		Jupiter::IRC::Client::Config->readFile(CONFIG_INI);
	}
	Jupiter::IRC::Client::data_->configSectionName = configSection;
	Jupiter::IRC::Client::data_->serverHostname = Jupiter::IRC::Client::readConfigValue("Hostname"_jrs, "irc.cncirc.net"_jrs);
	
	Jupiter::IRC::Client::data_->logFileName = Jupiter::IRC::Client::readConfigValue("LogFile"_jrs);
	Jupiter::IRC::Client::data_->nickname = Jupiter::IRC::Client::readConfigValue("Nick"_jrs, "Jupiter"_jrs);

	Jupiter::IRC::Client::data_->realName = Jupiter::IRC::Client::readConfigValue("Realname"_jrs, "Jupiter IRC Client"_jrs);

	Jupiter::IRC::Client::data_->saslPass = Jupiter::IRC::Client::readConfigValue("SASL.Password"_jrs);
	if (Jupiter::IRC::Client::data_->saslPass.isEmpty())
		Jupiter::IRC::Client::data_->saslPass = Jupiter::IRC::Client::readConfigValue("SASL.Pass"_jrs);
	
	Jupiter::IRC::Client::data_->saslAccount = Jupiter::IRC::Client::readConfigValue("SASL.Account"_jrs);
	if (Jupiter::IRC::Client::data_->saslAccount.isEmpty())
		Jupiter::IRC::Client::data_->saslAccount = Jupiter::IRC::Client::data_->nickname;

	Jupiter::IRC::Client::data_->autoPartMessage = Jupiter::IRC::Client::readConfigValue("AutoPartMessage"_jrs);

	Jupiter::IRC::Client::data_->ssl = Jupiter::IRC::Client::readConfigBool("SSL"_jrs);
	Jupiter::IRC::Client::data_->SSLCertificate = Jupiter::IRC::Client::readConfigValue("Certificate"_jrs);
	Jupiter::IRC::Client::data_->SSLKey = Jupiter::IRC::Client::readConfigValue("Key"_jrs);
	if (Jupiter::IRC::Client::data_->SSLCertificate.isEmpty())
	{
		Jupiter::IRC::Client::data_->SSLCertificate = Jupiter::IRC::Client::readConfigValue("Cert"_jrs);
		if (Jupiter::IRC::Client::data_->SSLCertificate.isEmpty())
			Jupiter::IRC::Client::data_->SSLCertificate = Jupiter::IRC::Client::data_->SSLKey;
	}
	if (Jupiter::IRC::Client::data_->SSLKey.isEmpty())
		Jupiter::IRC::Client::data_->SSLKey = Jupiter::IRC::Client::data_->SSLCertificate;

	Jupiter::IRC::Client::data_->joinOnKick = Jupiter::IRC::Client::readConfigBool("AutoJoinOnKick"_jrs);
	Jupiter::IRC::Client::data_->reconnectDelay = Jupiter::IRC::Client::readConfigInt("AutoReconnectDelay"_jrs);
	Jupiter::IRC::Client::data_->maxReconnectAttempts = Jupiter::IRC::Client::readConfigInt("MaxReconnectAttempts"_jrs);
	Jupiter::IRC::Client::data_->serverPort = (unsigned short)Jupiter::IRC::Client::readConfigInt("Port"_jrs, Jupiter::IRC::Client::data_->ssl ? 994 : 194);
	Jupiter::IRC::Client::data_->dChanType = Jupiter::IRC::Client::readConfigInt("Channel.Type"_jrs);

	if (Jupiter::IRC::Client::readConfigBool("PrintOutput"_jrs, true))
		Jupiter::IRC::Client::data_->printOutput = stdout;
	else
		Jupiter::IRC::Client::data_->printOutput = nullptr;
	if (Jupiter::IRC::Client::data_->logFileName.isNotEmpty())
		Jupiter::IRC::Client::data_->logFile = fopen(Jupiter::IRC::Client::data_->logFileName.c_str(), "a+b");
	else Jupiter::IRC::Client::data_->logFile = nullptr;

	if (Jupiter::IRC::Client::data_->ssl)
	{
		Jupiter::SecureTCPSocket *t = new Jupiter::SecureTCPSocket();
		if (Jupiter::IRC::Client::data_->SSLCertificate.isNotEmpty())
			t->setCertificate(Jupiter::IRC::Client::data_->SSLCertificate, Jupiter::IRC::Client::data_->SSLKey);
		Jupiter::IRC::Client::data_->sock = t;
	}
	else Jupiter::IRC::Client::data_->sock = new Jupiter::TCPSocket();

	Jupiter::IRC::Client::data_->connectionStatus = 0;
}

Jupiter::IRC::Client::~Client()
{
	Jupiter::IRC::Client::data_->sock->closeSocket();

	Jupiter::IRC::Client::data_->channels.emptyAndDelete();
	Jupiter::IRC::Client::data_->users.emptyAndDelete();

	if (Jupiter::IRC::Client::data_->sock != nullptr) delete Jupiter::IRC::Client::data_->sock;
	if (Jupiter::IRC::Client::data_->logFile != nullptr) fclose(Jupiter::IRC::Client::data_->logFile);
	delete Jupiter::IRC::Client::data_;
	Jupiter::IRC::Client::data_ = nullptr;
}

void Jupiter::IRC::Client::OnConnect()
{
	return;
}

void Jupiter::IRC::Client::OnDisconnect()
{
	return;
}

void Jupiter::IRC::Client::OnReconnectAttempt(bool)
{
	return;
}

void Jupiter::IRC::Client::OnRaw(const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnNumeric(long int, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnError(const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnChat(const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnNotice(const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnServerNotice(const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnCTCP(const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnAction(const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnInvite(const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnJoin(const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnPart(const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnNick(const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnKick(const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnQuit(const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

void Jupiter::IRC::Client::OnMode(const Jupiter::ReadableString &, const Jupiter::ReadableString &, const Jupiter::ReadableString &)
{
	return;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getConfigSection() const
{
	return Jupiter::IRC::Client::data_->configSectionName;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getLogFile() const
{
	return Jupiter::IRC::Client::data_->logFileName;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getPrefixes() const
{
	return Jupiter::IRC::Client::data_->prefixes;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getPrefixModes() const
{
	return Jupiter::IRC::Client::data_->prefixModes;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getNickname() const
{
	return Jupiter::IRC::Client::data_->nickname;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getRealname() const
{
	return Jupiter::IRC::Client::data_->realName;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getServerName() const
{
	return Jupiter::IRC::Client::data_->serverName;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getServerHostname() const
{
	return Jupiter::IRC::Client::data_->serverHostname;
}

unsigned short Jupiter::IRC::Client::getServerPort() const
{
	return Jupiter::IRC::Client::data_->serverPort;
}

time_t Jupiter::IRC::Client::getReconnectDelay() const
{
	return Jupiter::IRC::Client::data_->reconnectDelay;
}

time_t Jupiter::IRC::Client::getReconnectTime() const
{
	return Jupiter::IRC::Client::data_->reconnectTime;
}

int Jupiter::IRC::Client::getReconnectAttempts() const
{
	return Jupiter::IRC::Client::data_->reconnectAttempts;
}

int Jupiter::IRC::Client::getMaxReconnectAttempts() const
{
	return Jupiter::IRC::Client::data_->maxReconnectAttempts;
}

int Jupiter::IRC::Client::getDefaultChanType() const
{
	return Jupiter::IRC::Client::data_->dChanType;
}

FILE *Jupiter::IRC::Client::getPrintOutput() const
{
	return Jupiter::IRC::Client::data_->printOutput;
}

void Jupiter::IRC::Client::setPrintOutput(FILE *f)
{
	Jupiter::IRC::Client::data_->printOutput = f;
}

inline Jupiter::ReferenceString getSender(const Jupiter::ReadableString &line)
{
	return Jupiter::ReferenceString::getWord(line, 0, ":! ");
}

int Jupiter::IRC::Client::getAccessLevel(const Jupiter::ReadableString &chan, const Jupiter::ReadableString &nick) const
{
	Jupiter::IRC::Client::getChannelIndex(chan);
	unsigned int i = 0;
	Jupiter::IRC::Client::Channel *channel;
	while (i < Jupiter::IRC::Client::data_->channels.size())
	{
		channel = Jupiter::IRC::Client::data_->channels.get(i);
		if (channel->getName().equalsi(chan))
		{
			char prefix = channel->getUserPrefix(nick);
			if (prefix == 0) return 0;
			return Jupiter::IRC::Client::data_->prefixes.size() - Jupiter::IRC::Client::data_->prefixes.find(prefix);
		}
		i++;
	}
	return 0;
}

void Jupiter::IRC::Client::send(const Jupiter::ReadableString &rawMessage)
{
	Jupiter::StringS out = rawMessage;
	out += ENDL;
	Jupiter::IRC::Client::data_->sock->send(out);
}

Jupiter::IRC::Client::User *Jupiter::IRC::Client::getUser(unsigned int index) const
{
	return Jupiter::IRC::Client::data_->users.get(index);
}

Jupiter::IRC::Client::User *Jupiter::IRC::Client::getUser(const Jupiter::ReadableString &nickname) const
{
	Jupiter::IRC::Client::User *r;
	for (unsigned int i = 0; i < Jupiter::IRC::Client::data_->users.size(); i++)
	{
		r = Jupiter::IRC::Client::data_->users.get(i);
		if (r->getNickname().equalsi(nickname)) return r;
	}
	return nullptr;
}

int Jupiter::IRC::Client::getUserIndex(const Jupiter::ReadableString &nickname) const
{
	for (int i = Jupiter::IRC::Client::data_->users.size() - 1; i >= 0; i--) if (Jupiter::IRC::Client::data_->users.get(i)->getNickname().equalsi(nickname)) return i;
	return -1;
}

int Jupiter::IRC::Client::getUserIndex(Jupiter::IRC::Client::User *user) const
{
	for (int i = Jupiter::IRC::Client::data_->users.size() - 1; i >= 0; i--) if (Jupiter::IRC::Client::data_->users.get(i) == user) return i;
	return -1;
}

unsigned int Jupiter::IRC::Client::getUserCount() const
{
	return Jupiter::IRC::Client::data_->users.size();
}

unsigned int Jupiter::IRC::Client::getChannelCount() const
{
	return Jupiter::IRC::Client::data_->channels.size();
}

Jupiter::IRC::Client::Channel *Jupiter::IRC::Client::getChannel(unsigned int index) const
{
	return Jupiter::IRC::Client::data_->channels.get(index);
}

Jupiter::IRC::Client::Channel *Jupiter::IRC::Client::getChannel(const Jupiter::ReadableString &chanName) const
{
	int index = Jupiter::IRC::Client::getChannelIndex(chanName);
	if (index < 0) return nullptr;
	return Jupiter::IRC::Client::getChannel(index);
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getChannelName(unsigned int index) const
{
	return Jupiter::IRC::Client::data_->channels.get(index)->getName();
}

int Jupiter::IRC::Client::getChannelIndex(const Jupiter::ReadableString &chanName) const
{
	for (size_t i = 0; i < Jupiter::IRC::Client::data_->channels.size(); i++)
		if (Jupiter::IRC::Client::data_->channels.get(i)->getName().equalsi(chanName)) return i;
	return -1;
}

bool Jupiter::IRC::Client::isAutoReconnect() const
{
	return Jupiter::IRC::Client::data_->maxReconnectAttempts != 0;
}

void Jupiter::IRC::Client::setAutoReconnect(int val)
{
	Jupiter::IRC::Client::data_->maxReconnectAttempts = val;
}

void Jupiter::IRC::Client::joinChannel(const Jupiter::ReadableString &channel)
{
	Jupiter::IRC::Client::data_->sock->send(Jupiter::StringS::Format("JOIN %.*s" ENDL, channel.size(), channel.ptr()));
}

void Jupiter::IRC::Client::joinChannel(const Jupiter::ReadableString &channel, const Jupiter::ReadableString &password)
{
	Jupiter::IRC::Client::data_->sock->send(Jupiter::StringS::Format("JOIN %.*s %.*s" ENDL, channel.size(), channel.ptr(), password.size(), password.ptr()));
}

void Jupiter::IRC::Client::partChannel(const Jupiter::ReadableString &channel)
{
	Jupiter::IRC::Client::data_->sock->send(Jupiter::StringS::Format("PART %.*s" ENDL, channel.size(), channel.ptr()));
	int index = Jupiter::IRC::Client::getChannelIndex(channel);
	if (index >= 0) Jupiter::IRC::Client::data_->channels.get(index)->setType(-2);
}

void Jupiter::IRC::Client::partChannel(const Jupiter::ReadableString &channel, const Jupiter::ReadableString &message)
{
	Jupiter::IRC::Client::data_->sock->send(Jupiter::StringS::Format("PART %.*s :%.*s" ENDL, channel.size(), channel.ptr(), message.size(), message.ptr()));
	int index = Jupiter::IRC::Client::getChannelIndex(channel);
	if (index >= 0) Jupiter::IRC::Client::data_->channels.get(index)->setType(-2);
}

void Jupiter::IRC::Client::sendMessage(const Jupiter::ReadableString &dest, const Jupiter::ReadableString &message)
{
	Jupiter::IRC::Client::data_->sock->send(Jupiter::StringS::Format("PRIVMSG %.*s :%.*s" ENDL, dest.size(), dest.ptr(), message.size(), message.ptr()));
}

void Jupiter::IRC::Client::sendNotice(const Jupiter::ReadableString &dest, const Jupiter::ReadableString &message)
{
	Jupiter::IRC::Client::data_->sock->send(Jupiter::StringS::Format("NOTICE %.*s :%.*s" ENDL, dest.size(), dest.ptr(), message.size(), message.ptr()));
}

unsigned int Jupiter::IRC::Client::messageChannels(int type, const Jupiter::ReadableString &message)
{
	unsigned int total = 0;
	Jupiter::IRC::Client::Channel *channel;
	for (unsigned int i = 0; i < Jupiter::IRC::Client::data_->channels.size(); i++)
	{
		channel = Jupiter::IRC::Client::data_->channels.get(i);
		if (channel->getType() == type)
		{
			Jupiter::IRC::Client::sendMessage(channel->getName(), message);
			total++;
		}
	}
	return total;
}

unsigned int Jupiter::IRC::Client::messageChannels(const Jupiter::ReadableString &message)
{
	unsigned int total = 0;
	Jupiter::IRC::Client::Channel *channel;
	for (unsigned int i = 0; i < Jupiter::IRC::Client::data_->channels.size(); i++)
	{
		channel = Jupiter::IRC::Client::data_->channels.get(i);
		if (channel->getType() >= 0)
		{
			Jupiter::IRC::Client::sendMessage(channel->getName(), message);
			total++;
		}
	}
	return total;
}

int Jupiter::IRC::Client::primaryHandler()
{
	if (Jupiter::IRC::Client::data_->connectionStatus == 0) return -1;
	int recvVal = Jupiter::IRC::Client::data_->sock->recv();
	if (recvVal > 0)
	{
		Jupiter::IRC::Client::data_->buffer = Jupiter::IRC::Client::data_->sock->getBuffer();
		unsigned int totalLines = Jupiter::IRC::Client::data_->buffer.wordCount(ENDL);
		for (unsigned int currentLine = 0; currentLine < totalLines; currentLine++)
		{
			Jupiter::ReferenceString buff = Jupiter::IRC::Client::data_->buffer.getWord(currentLine, ENDL);
			if (buff.isNotEmpty())
			{
				Jupiter::IRC::Client::writeToLogs(buff);
				if (Jupiter::IRC::Client::data_->printOutput != nullptr)
				{
					buff.println(Jupiter::IRC::Client::data_->printOutput);
				}
				Jupiter::ReferenceString w1 = Jupiter::ReferenceString::getWord(buff, 0, WHITESPACE);
				if (w1.isNotEmpty())
				{
					Jupiter::ReferenceString w2 = Jupiter::ReferenceString::getWord(buff, 1, WHITESPACE);
					int numeric = w2.asInt(10);
					if (w1[0] == ':') //Messages
					{
						if (w2.isNotEmpty())
						{
							switch (numeric) // Numerics that don't rely on a specific connectionStatus.
							{
							case IRC_RPL_BOUNCE: // 010
							{
								Jupiter::ReferenceString portToken = Jupiter::ReferenceString::getWord(buff, 4, " ");
								unsigned short port;
								if (portToken[0] == '+') // This is most likely not used anywhere.
								{
									port = (unsigned short)portToken.asUnsignedInt(10);
									if (Jupiter::IRC::Client::data_->ssl == false)
									{
										Jupiter::IRC::Client::data_->ssl = true;
										delete Jupiter::IRC::Client::data_->sock;
										Jupiter::IRC::Client::data_->sock = new Jupiter::SecureTCPSocket();
									}
								}
								else
								{
									port = (unsigned short)portToken.asUnsignedInt(10);
									if (Jupiter::IRC::Client::data_->ssl == true)
									{
										Jupiter::IRC::Client::data_->ssl = false;
										delete Jupiter::IRC::Client::data_->sock;
										Jupiter::IRC::Client::data_->sock = new Jupiter::TCPSocket();
									}
								}
								if (port != 0) // Don't default -- could be non-compliant input.
								{
									Jupiter::IRC::Client::data_->serverHostname = Jupiter::ReferenceString::getWord(buff, 3, WHITESPACE);
									Jupiter::IRC::Client::data_->serverPort = port;
									puts("Reconnecting due to old bounce.");
									this->reconnect();
								}
								else puts("Error: Failed to parse bounce token.");
							}
								break;
							} // numeric switch
							switch (Jupiter::IRC::Client::data_->connectionStatus)
							{
							case 1: // Socket established -- attempting STARTTLS
								switch (numeric)
								{
								case IRC_RPL_BOUNCEOLD: // 005
									if (buff.matchi("*:Try server *, port *"))
									{
										Jupiter::ReferenceString portToken = Jupiter::ReferenceString::getWord(buff, 6, " ");
										unsigned short bouncePort = (unsigned short)Jupiter::ReferenceString::getWord(buff, 6, " ").asInt(10);

										if (portToken[0] == '+') // This is almost certainly not used anywhere.
										{
											bouncePort = (unsigned short)portToken.asInt(10);
											if (Jupiter::IRC::Client::data_->ssl == false)
											{
												Jupiter::IRC::Client::data_->ssl = true;
												delete Jupiter::IRC::Client::data_->sock;
												Jupiter::IRC::Client::data_->sock = new Jupiter::SecureTCPSocket();
											}
										}
										else
										{
											bouncePort = (unsigned short)portToken.asInt(10);
											if (Jupiter::IRC::Client::data_->ssl == true)
											{
												Jupiter::IRC::Client::data_->ssl = false;
												delete Jupiter::IRC::Client::data_->sock;
												Jupiter::IRC::Client::data_->sock = new Jupiter::TCPSocket();
											}
										}
										if (bouncePort != 0)
										{
											Jupiter::IRC::Client::data_->serverHostname = Jupiter::ReferenceString::getWord(buff, 4, " ");
											Jupiter::IRC::Client::data_->serverHostname.truncate(1); // trailing comma
											Jupiter::IRC::Client::data_->serverPort = bouncePort;
											puts("Reconnecting due to old bounce.");
											this->reconnect();
										}
										else puts("Error: Failed to parse old bounce token.");
									}
									break;
								case IRC_ERR_UNKNOWNCOMMAND: // 421
								{
									Jupiter::ReferenceString command = Jupiter::ReferenceString::getWord(buff, 2, " ");
									if (command.equalsi("STARTTLS")) // Server doesn't support STARTTLS
									{
										Jupiter::IRC::Client::data_->startCAP();
									}
								}
									break;
								case IRC_RPL_STARTTLS: // 670
								{
									Jupiter::SecureTCPSocket *t = new Jupiter::SecureTCPSocket(std::move(*Jupiter::IRC::Client::data_->sock));
									delete Jupiter::IRC::Client::data_->sock;
									Jupiter::IRC::Client::data_->sock = t;
									Jupiter::IRC::Client::data_->ssl = true;
									// toggle blocking to prevent error
									if (Jupiter::IRC::Client::data_->SSLCertificate.isNotEmpty())
										t->setCertificate(Jupiter::IRC::Client::data_->SSLCertificate, Jupiter::IRC::Client::data_->SSLKey);

									bool goodSSL;
									if (t->getBlockingMode() == false)
									{
										t->setBlocking(true);
										goodSSL = t->initSSL();
										t->setBlocking(false);
									}
									else goodSSL = t->initSSL();

									if (goodSSL) Jupiter::IRC::Client::data_->startCAP();
									else
									{
										// Something went wrong. Kill the socket.
										t->closeSocket();
									}
								}
									break;
								case IRC_ERR_STARTTLS: // 691
									Jupiter::IRC::Client::data_->startCAP();
									break;
								default:
									break;
								} // numeric switch
								break;

							case 2: // Capability negotiation
								switch (numeric)
								{
								case 0:
									if (w2.equalsi("CAP"))
									{
										Jupiter::ReferenceString w4 = Jupiter::ReferenceString::getWord(buff, 3, WHITESPACE);
										if (w4.equals("LS"))
										{
											Jupiter::ReferenceString listParams = buff.gotoWord(4, WHITESPACE);
											if (listParams[0] == ':') listParams.shiftRight(1);
											unsigned int len = listParams.wordCount(WHITESPACE);
											Jupiter::ReferenceString curr;
											Jupiter::StringL req = "CAP REQ :";
											bool sasl = false;
											for (unsigned int i = 0; i < len; i++)
											{
												curr = listParams.getWord(i, WHITESPACE);
												if (curr.equalsi("multi-prefix")) req += "multi-prefix ";
												else if (curr.equalsi("userhost-in-names")) req += "userhost-in-names ";
												else if (curr.equalsi("sasl"))
												{
													if (Jupiter::IRC::Client::data_->saslPass.isNotEmpty())
													{
														req += "sasl ";
														sasl = true;
													}
												}
												// else; // We don't know what this is!
											}
											if (req.size() > 9)
											{
												req -= 1; // Trim off the extra space byte.
												req += ENDL;
												Jupiter::IRC::Client::data_->sock->send(req);
												if (sasl)
													Jupiter::IRC::Client::data_->sock->send("AUTHENTICATE PLAIN" ENDL);
											}
											if (!sasl)
											{
												Jupiter::IRC::Client::data_->sock->send(STRING_LITERAL_AS_REFERENCE("CAP END" ENDL));
												Jupiter::IRC::Client::data_->registerClient();
											}
										}
									}
									break;
								case IRC_ERR_UNKNOWNCOMMAND: // 421
									if (w2.equalsi("CAP")) // Server doesn't support CAP
									{
										Jupiter::IRC::Client::data_->registerClient();
									}
									break;
								default:
									break;
								} // numeric switch
								break;

							case 3: // Registration sent, but not verified.
							{
								bool completelyBadNick = false;
								switch (numeric)
								{
									// We'll take any of these 4, just in-case any of them are missing. In general, this will trigger on 001.
								case IRC_RPL_MYINFO: // 004
									Jupiter::IRC::Client::data_->serverName = Jupiter::ReferenceString::getWord(buff, 3, " ");
								case IRC_RPL_WELCOME: // 001
								case IRC_RPL_YOURHOST: // 002
								case IRC_RPL_CREATED: // 003
									Jupiter::IRC::Client::data_->connectionStatus = 4;
									break;

									// You have a bad nickname! Try the alt.
									//case IRC_ERR_NONICKNAMEGIVEN: // 431 -- Not consistently usable due to lack of command field.
								case IRC_ERR_ERRONEOUSNICKNAME: // 432
									completelyBadNick = true;
								case IRC_ERR_NICKNAMEINUSE: // 433
								case IRC_ERR_NICKCOLLISION: // 436
								case IRC_ERR_BANNICKCHANGE: // 437 -- Note: This conflicts with another token.
									const Jupiter::ReadableString &altNick = Jupiter::IRC::Client::readConfigValue("AltNick"_jrs, "Jupiter"_jrs);
									const Jupiter::ReadableString &configNick = Jupiter::IRC::Client::readConfigValue("Nick"_jrs, "Jupiter"_jrs);
									/*
									* Possible Issues:
									*	altNick != nickname after first nick change -- loop initiated?
									*	completelyBadNick will apply to altNick if it was tried, not the actual nick.
									*/
									if (Jupiter::IRC::Client::data_->nickname.equalsi(altNick)) // The alternate nick failed.
									{
										Jupiter::IRC::Client::data_->nickname = configNick;
										Jupiter::IRC::Client::data_->nickname += "1";
									}
									else if (Jupiter::IRC::Client::data_->nickname.equalsi(configNick))
									{
										Jupiter::IRC::Client::data_->nickname = altNick;
										if (Jupiter::IRC::Client::data_->nickname.isNotEmpty())
											Jupiter::IRC::Client::data_->sock->send(Jupiter::StringS::Format("NICK %.*s" ENDL, Jupiter::IRC::Client::data_->nickname.size(), Jupiter::IRC::Client::data_->nickname.ptr()));
									}
									// Note: Add a series of contains() functions to String_Type.
									else
									{
										if (completelyBadNick == false) // If this nick is invalid, adding numbers won't help.
										{
											if (Jupiter::IRC::Client::data_->nickname.size() > configNick.size())
											{
												int n = Jupiter_strtoi_nospace_s(Jupiter::IRC::Client::data_->nickname.ptr() + configNick.size(), Jupiter::IRC::Client::data_->nickname.size() - configNick.size(), 10);
												Jupiter::IRC::Client::data_->nickname.format("%.*s%d", configNick.size(), configNick.ptr(), n);
											}
											else
											{
												// Something strange is going on -- did somebody rehash?
												// This can be somewhat edgy -- this will only trigger if someone rehashes AND the new nickname is shorter.
												// However, it won't be fatal even if the new nickname's length is >= the old.
												Jupiter::IRC::Client::data_->nickname = configNick;
											}
										}
										else
										{
											// Disconnect and don't try again.
											// Consider passing this to plugins so that they can figure it out (i.e: a plugin could display a prompt and ask for input).
										}
									}
									break;
								}
							}
								break;

							case 4: // Registration verified, but connection process in progress.
								switch (numeric)
								{
								case IRC_RPL_ISUPPORT: // 005
								{
									size_t pos = buff.find("PREFIX=("_jrs);
									if (pos != Jupiter::INVALID_INDEX)
									{
										Jupiter::ReferenceString ref = Jupiter::ReferenceString::substring(buff, pos + 8);
										Jupiter::IRC::Client::data_->prefixModes = Jupiter::ReferenceString::getWord(ref, 0, ")");
										ref.shiftRight(ref.find(')') + 1);
										Jupiter::IRC::Client::data_->prefixes = Jupiter::ReferenceString::getWord(ref, 0, " " ENDL);
									}
									pos = buff.find("CHANMODES="_jrs);
									if (pos != Jupiter::INVALID_INDEX)
									{
										Jupiter::ReferenceString ref = Jupiter::ReferenceString::substring(buff, pos + 10);
										ref = ref.getWord(0, " ");
										size_t pos2 = ref.find(',', 0);
										if (pos != INVALID_INDEX)
										{
											Jupiter::IRC::Client::data_->modeA = ref.getWord(0, ", ");
											ref.shiftRight(pos + 1);
											pos2 = ref.find(',', 0);
											if (pos != INVALID_INDEX)
											{
												Jupiter::IRC::Client::data_->modeB = ref.getWord(0, ", ");
												ref.shiftRight(pos + 1);
												pos2 = ref.find(',', 0);
												if (pos != INVALID_INDEX)
												{
													Jupiter::IRC::Client::data_->modeC = ref.getWord(0, ", ");
													ref.shiftRight(pos + 1);
													Jupiter::IRC::Client::data_->modeD = ref.getWord(0, ", ");
												}
											}
										}
									}
									pos = buff.find("CHANTYPES="_jrs);
									if (pos != Jupiter::INVALID_INDEX)
									{
										Jupiter::ReferenceString ref = Jupiter::ReferenceString::substring(buff, pos + 10);
										Jupiter::IRC::Client::data_->chanTypes = ref.getWord(0, " ");
									}
								}
									break;
								case IRC_RPL_LUSERCLIENT: // 251
								{
									Jupiter::StringL key = "RawData.";
									size_t offset;

									unsigned int i = 1;
									Jupiter::ReferenceString value;
									auto config_loop_condition = [&]
									{
										offset = key.aformat("%u", i);
										value = Jupiter::IRC::Client::readConfigValue(key);
										return !value.isEmpty();
									};
									while (config_loop_condition())
									{
										key.truncate(offset);
										Jupiter::IRC::Client::send(value);
										i++;
									}
									key = "Channel.";
									i = 1;
									while (config_loop_condition())
									{
										key.truncate(offset);
										Jupiter::IRC::Client::joinChannel(value);
										i++;
									}

									Jupiter::IRC::Client::data_->connectionStatus = 5;
									Jupiter::IRC::Client::data_->reconnectAttempts = 0;
									this->OnConnect();
									for (i = 0; i < Jupiter::plugins->size(); i++) Jupiter::plugins->get(i)->OnConnect(this);
								}
									break;
								}
								break;

							default: // Post-registration.
								if (w2.equalsi("PRIVMSG"))
								{
									Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(buff, 2, WHITESPACE);
									if (chan.isNotEmpty())
									{
										Jupiter::ReferenceString nick = getSender(buff);
										if (nick.isNotEmpty())
										{
											Jupiter::ReferenceString premessage = Jupiter::ReferenceString::substring(buff, buff.find(':', 1) + 1);
											if (premessage[0] == '\001') //CTCP (ACTIONs are included)
											{
												Jupiter::ReferenceString rawmessage(premessage.ptr() + 1, premessage.size() - 1);
												Jupiter::ReferenceString command = rawmessage.getWord(0, WHITESPACE);
												if (command[command.size() - 1] == IRC::CTCP) command.truncate(1);
												Jupiter::ReferenceString message = rawmessage.substring(rawmessage.find(' ') + 1, rawmessage.find(IRC::CTCP));
												if (message[message.size() - 1] == IRC::CTCP) message.truncate(1);

												if (command.equals("ACTION"))
												{
													this->OnAction(chan, nick, message);
													for (size_t i = 0; i < Jupiter::plugins->size(); i++)
														Jupiter::plugins->get(i)->OnAction(this, chan, nick, message);
												}
												else
												{
													Jupiter::StringL response = "NOTICE ";
													response += nick;
													response += " :" IRCCTCP;
													response += command;
													response += ' ';
													if (command.equals("PING")) response += message;
													else if (command.equals("VERSION")) response += Jupiter::version;
													else if (command.equals("FINGER")) response += "Oh, yeah, a little to the left.";
													else if (command.equals("SOURCE")) response += "https://github.com/JAJames/Jupiter";
													else if (command.equals("USERINFO")) response += "Hey, I'm Jupiter! If you have questions, ask Agent! (irc.cncirc.net)";
													else if (command.equals("CLIENTINFO")) response += "I'll tell you what I don't know: This command!";
													else if (command.equals("TIME")) response += getTime();
													else if (command.equals("ERRMSG")) response += message;
													else
													{
														response = "NOTICE ";
														response += nick;
														response += " :" IRCCTCP "ERRMSG ";
														response += command;
														response += " :Query is unknown";
													}
													response += IRCCTCP ENDL;
													Jupiter::IRC::Client::data_->sock->send(response);
													this->OnCTCP(chan, nick, command, message);
													for (size_t i = 0; i < Jupiter::plugins->size(); i++)
														Jupiter::plugins->get(i)->OnCTCP(this, chan, nick, message);
												}
											}
											else
											{
												Jupiter::ReferenceString message = premessage;
												this->OnChat(chan, nick, message);
												for (size_t i = 0; i < Jupiter::plugins->size(); i++)
													Jupiter::plugins->get(i)->OnChat(this, chan, nick, message);
											}
										}
									}
								}
								else if (w2.equalsi("NOTICE"))
								{
									Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(buff, 2, WHITESPACE);
									if (chan.isNotEmpty())
									{
										size_t pos = buff.find('!', 0);
										auto message = Jupiter::ReferenceString::substring(buff, buff.find(':', 1) + 1, buff.size());
										if (pos < buff.find(' '))
										{
											auto nick = Jupiter::ReferenceString::substring(buff, 1, pos);
											this->OnNotice(chan, nick, message);
											for (size_t i = 0; i < Jupiter::plugins->size(); i++)
												Jupiter::plugins->get(i)->OnNotice(this, chan, nick, message);
										}
										else
										{
											auto sender = getSender(buff);
											if (sender.isNotEmpty())
											{
												this->OnServerNotice(chan, sender, message);
												for (size_t i = 0; i < Jupiter::plugins->size(); i++)
													Jupiter::plugins->get(i)->OnServerNotice(this, chan, sender, message);
											}
										}
									}
								}
								else if (w2.equalsi("NICK"))
								{
									auto nick = getSender(buff);
									Jupiter::ReferenceString newnick = Jupiter::ReferenceString::substring(buff, buff.find(' ', 1) + 1);
									if (newnick.isNotEmpty() && newnick[0] == ':') newnick.shiftRight(1);
									if (nick.equalsi(Jupiter::IRC::Client::data_->nickname))
									{
										Jupiter::IRC::Client::data_->nickname = newnick;
									}
									Jupiter::IRC::Client::User *user = Jupiter::IRC::Client::data_->findUser(nick);
									if (user != nullptr)
									{
										user->data_->nickname = newnick;
										this->OnNick(nick, newnick);
									}
									for (size_t i = 0; i < Jupiter::plugins->size(); i++)
										Jupiter::plugins->get(i)->OnNick(this, nick, newnick);
								}
								else if (w2.equalsi("JOIN"))
								{
									auto nick = getSender(buff);
									Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(buff, 2, WHITESPACE);
									if (chan[0] == ':') chan.shiftRight(1);
									int i = Jupiter::IRC::Client::getChannelIndex(chan);
									if (Jupiter::IRC::Client::data_->nickname.equalsi(nick))
									{
										if (i >= 0) Jupiter::IRC::Client::data_->delChannel(i);
										Jupiter::IRC::Client::Channel *channel = Jupiter::IRC::Client::data_->channels.get(Jupiter::IRC::Client::data_->addChannel(chan));
										channel->data_->isAddingNames = true;
										if (channel->getType() < 0)
										{
											if (Jupiter::IRC::Client::data_->autoPartMessage.isNotEmpty())
												Jupiter::IRC::Client::partChannel(chan, Jupiter::IRC::Client::data_->autoPartMessage);
											else
												Jupiter::IRC::Client::partChannel(chan);
										}
									}
									else if (i >= 0) Jupiter::IRC::Client::data_->channels.get(i)->addUser(Jupiter::IRC::Client::data_->findUserOrAdd(nick));
									this->OnJoin(chan, nick);
									for (i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnJoin(this, chan, nick);
								}
								else if (w2.equalsi("PART"))
								{
									auto nick = getSender(buff);
									if (nick.isNotEmpty())
									{
										Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(buff, 2, WHITESPACE);
										if (chan.isNotEmpty())
										{
											int i = Jupiter::IRC::Client::getChannelIndex(chan);
											if (i >= 0)
											{
												int userIndex = Jupiter::IRC::Client::getUserIndex(nick);
												if (userIndex >= 0)
												{
													Jupiter::IRC::Client::User *user = Jupiter::IRC::Client::data_->users.get(userIndex);
													Jupiter::IRC::Client::data_->channels.get(i)->delUser(nick);
													Jupiter::ReferenceString reason;
													size_t pos = buff.find(':', 1);
													if (pos != Jupiter::INVALID_INDEX)
														reason = Jupiter::ReferenceString::substring(buff, pos + 1);
													this->OnPart(chan, nick, reason);
													for (i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnPart(this, chan, nick, reason);
													if (nick.equalsi(Jupiter::IRC::Client::data_->nickname)) Jupiter::IRC::Client::data_->delChannel(chan);
													if (user->getChannelCount() == 0) delete Jupiter::IRC::Client::data_->users.remove(userIndex);
												}
											}
										}
									}
								}
								else if (w2.equalsi("KICK"))
								{
									Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(buff, 2, WHITESPACE);
									if (chan.isNotEmpty())
									{
										Jupiter::ReferenceString kicker = getSender(buff);
										if (kicker.isNotEmpty())
										{
											Jupiter::ReferenceString kicked = Jupiter::ReferenceString::getWord(buff, 3, WHITESPACE);
											if (kicked.isNotEmpty())
											{
												int i = Jupiter::IRC::Client::getChannelIndex(chan);
												if (i >= 0)
												{
													int userIndex = Jupiter::IRC::Client::getUserIndex(kicked);
													if (userIndex >= 0)
													{
														Jupiter::IRC::Client::User *user = Jupiter::IRC::Client::data_->users.get(userIndex);
														Jupiter::IRC::Client::data_->channels.get(i)->delUser(kicked);
														size_t pos = buff.find(':', 1);
														Jupiter::ReferenceString reason;
														if (pos != Jupiter::INVALID_INDEX)
															reason = Jupiter::ReferenceString::substring(buff, pos + 1);
														this->OnKick(chan, kicker, kicked, reason);
														for (i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnKick(this, chan, kicker, kicked, reason);
														if (kicked.equalsi(Jupiter::IRC::Client::data_->nickname))
														{
															Jupiter::IRC::Client::data_->delChannel(chan);
															if (Jupiter::IRC::Client::data_->joinOnKick) Jupiter::IRC::Client::joinChannel(chan);
														}
														if (user->getChannelCount() == 0) delete Jupiter::IRC::Client::data_->users.remove(userIndex);
													}
												}
											}
										}
									}
								}
								else if (w2.equalsi("QUIT"))
								{
									Jupiter::ReferenceString nick = getSender(buff);
									Jupiter::ReferenceString message = Jupiter::ReferenceString::substring(buff, buff.find(':', 1) + 1);
									int userIndex = Jupiter::IRC::Client::getUserIndex(nick);
									if (userIndex >= 0)
									{
										Jupiter::IRC::Client::User *user = Jupiter::IRC::Client::data_->users.get(userIndex);
										unsigned int i;
										for (i = 0; i < Jupiter::IRC::Client::data_->channels.size(); i++)
											Jupiter::IRC::Client::data_->channels.get(i)->delUser(nick);
										this->OnQuit(nick, message);
										for (i = 0; i < Jupiter::plugins->size(); i++) Jupiter::plugins->get(i)->OnQuit(this, nick, message);
										if (user->getChannelCount() == 0) delete Jupiter::IRC::Client::data_->users.remove(userIndex);
									}
								}
								else if (w2.equalsi("INVITE"))
								{
									Jupiter::ReferenceString inviter = getSender(buff);
									Jupiter::ReferenceString invited = Jupiter::ReferenceString::getWord(buff, 2, WHITESPACE);
									Jupiter::ReferenceString chan = Jupiter::ReferenceString::substring(buff, buff.find(':', 1) + 1);
									this->OnInvite(chan, inviter, invited);
									for (size_t i = 0; i < Jupiter::plugins->size(); i++)
										Jupiter::plugins->get(i)->OnInvite(this, chan, inviter, invited);
								}
								else if (w2.equalsi("MODE"))
								{
									Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(buff, 2, WHITESPACE);
									if (chan.isNotEmpty())
									{
										if (Jupiter::IRC::Client::data_->chanTypes.contains(chan[0]))
										{
											auto nick = getSender(buff);
											if (nick.isNotEmpty())
											{
												Jupiter::ReferenceString modestring = Jupiter::ReferenceString::substring(buff, buff.find(' ', 2) + 1);
												if (modestring.wordCount(" ") > 1)
												{
													Jupiter::ReferenceString modes = modestring.getWord(0, " ");
													if (modes.isNotEmpty())
													{
														modestring.shiftRight(modestring.find(' ') + 1);
														Jupiter::ReferenceString tword;
														unsigned char g = 0;
														char symb = 0;
														for (uint8_t z = 0; z != modes.size(); z++)
														{
															if (modes[z] == '+' || modes[z] == '-')
																symb = modes[z];
															else if (Jupiter::IRC::Client::data_->prefixModes.contains(modes[z])) // user prefix mode
															{

																tword = modestring.getWord(g, " ");
																if (tword.isNotEmpty())
																{
																	Jupiter::IRC::Client::Channel *channel;
																	for (unsigned int channelIndex = 0; channelIndex < Jupiter::IRC::Client::data_->channels.size(); channelIndex++)
																	{
																		channel = Jupiter::IRC::Client::data_->channels.get(channelIndex);
																		if (channel->getName().equalsi(chan))
																		{
																			if (symb == '+')
																				channel->addUserPrefix(Jupiter::ReferenceString(tword), Jupiter::IRC::Client::data_->prefixes[Jupiter::IRC::Client::data_->prefixModes.find(modes[z])]);
																			else channel->delUserPrefix(Jupiter::ReferenceString(tword), Jupiter::IRC::Client::data_->prefixes[Jupiter::IRC::Client::data_->prefixModes.find(modes[z])]);
																			break;
																		}
																	}
																}
																g++;
															}
															else if (Jupiter::IRC::Client::data_->modeA.contains(modes[z])) // mode type A
																g++;
															else if (Jupiter::IRC::Client::data_->modeB.contains(modes[z])) // mode type B
																g++;
															else if (Jupiter::IRC::Client::data_->modeC.contains(modes[z]) && symb == '+') // mode type C (with parameter)
																g++;
															// else; // mode type D
														}
													}
												}
												this->OnMode(chan, nick, modestring);
												for (size_t i = 0; i < Jupiter::plugins->size(); i++)
													Jupiter::plugins->get(i)->OnMode(this, chan, nick, modestring);
											}
										}
									}
								}
								// else if ACCOUNT
								// else if CHGHOST
								else if (numeric == IRC_RPL_NAMREPLY) // Some names.
								{
									Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(buff, 4, " ");
									Jupiter::ReferenceString names = Jupiter::ReferenceString::substring(buff, buff.find(':', 1) + 1);
									int i = Jupiter::IRC::Client::getChannelIndex(chan);
									if (i >= 0)
									{
										if (Jupiter::IRC::Client::getChannel(i)->data_->isAddingNames == false)
										{
											Jupiter::IRC::Client::data_->delChannel(i);
											Jupiter::IRC::Client::getChannel(Jupiter::IRC::Client::data_->addChannel(chan))->data_->isAddingNames = true;
										}
										Jupiter::IRC::Client::data_->addNamesToChannel(i, names);
									}
								}
								else if (numeric == IRC_RPL_ENDOFNAMES) // We're done here.
								{
									Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(buff, 3, " ");
									int i = Jupiter::IRC::Client::getChannelIndex(chan);
									if (i >= 0) Jupiter::IRC::Client::data_->channels.get(i)->data_->isAddingNames = false;
								}
								break;
							}
						}
					}
					else
					{
						if (w1.equals("PING"))
						{
							Jupiter::IRC::Client::data_->sock->send(Jupiter::StringS::Format("PONG %.*s" ENDL, w2.size(), w2.ptr()));
						}
						else if (w1.equals("NICK"))
						{
							if (w2.isNotEmpty())
								Jupiter::IRC::Client::data_->nickname = w2;
						}
						else if (w1.equals("ERROR"))
						{
							Jupiter::ReferenceString reason = Jupiter::ReferenceString::substring(buff, buff.find(':') + 1);
							this->OnError(reason);
							for (size_t i = 0; i < Jupiter::plugins->size(); i++)
								Jupiter::plugins->get(i)->OnError(this, reason);
							Jupiter::IRC::Client::disconnect();
						}
						else if (w1.equals("AUTHENTICATE"))
						{
							if (Jupiter::IRC::Client::data_->saslPass.isNotEmpty())
							{
								size_t authStringLen = Jupiter::IRC::Client::data_->nickname.size() + Jupiter::IRC::Client::data_->saslAccount.size() + Jupiter::IRC::Client::data_->saslPass.size() + 2;
								char *authString = new char[authStringLen + 1];
								int offset = sprintf(authString, "%.*s", Jupiter::IRC::Client::data_->nickname.size(), Jupiter::IRC::Client::data_->nickname.ptr()) + 1;
								offset += sprintf(authString + offset, "%.*s", Jupiter::IRC::Client::data_->saslAccount.size(), Jupiter::IRC::Client::data_->saslAccount.ptr()) + 1;
								offset += sprintf(authString + offset, "%.*s", Jupiter::IRC::Client::data_->saslPass.size(), Jupiter::IRC::Client::data_->saslPass.ptr());

								char *enc = Jupiter::base64encode(authString, authStringLen);
								delete[] authString;

								char *out = new char[strlen(enc) + 16];
								int len = sprintf(out, "AUTHENTICATE %s" ENDL, enc);
								delete[] enc;
								Jupiter::IRC::Client::data_->sock->send(out, len);
								delete[] out;
							}
							Jupiter::IRC::Client::data_->sock->send("CAP END" ENDL);
							Jupiter::IRC::Client::data_->registerClient();
						}
					}
					if (numeric != 0)
					{
						this->OnNumeric(numeric, buff);
						for (size_t i = 0; i < Jupiter::plugins->size(); i++)
							Jupiter::plugins->get(i)->OnNumeric(this, numeric, buff);
					}
				}
				this->OnRaw(buff);
				for (size_t i = 0; i < Jupiter::plugins->size(); i++)
					Jupiter::plugins->get(i)->OnRaw(this, buff);
			}
		}
		return 0;
	}
	int lastError = Jupiter::IRC::Client::data_->sock->getLastError();
	if (lastError == 10035)
		return 0;
	if (Jupiter::IRC::Client::data_->connectionStatus)
		Jupiter::IRC::Client::disconnect();
	return lastError;
}

bool Jupiter::IRC::Client::connect()
{
	const Jupiter::ReadableString &clientAddress = Jupiter::IRC::Client::readConfigValue("ClientAddress"_jrs);
	if (Jupiter::IRC::Client::data_->sock->connect(Jupiter::IRC::Client::data_->serverHostname.c_str(), Jupiter::IRC::Client::data_->serverPort, clientAddress.isEmpty() ? nullptr : Jupiter::CStringS(clientAddress).c_str(), (unsigned short)Jupiter::IRC::Client::readConfigLong("ClientPort"_jrs)) == false)
		return false;

	Jupiter::IRC::Client::data_->sock->setBlocking(false);
	if (Jupiter::IRC::Client::data_->ssl == false && Jupiter::IRC::Client::readConfigBool("STARTTLS"_jrs, true))
	{
		Jupiter::IRC::Client::data_->sock->send("STARTTLS" ENDL);
		Jupiter::IRC::Client::data_->connectionStatus = 1;
	}
	else Jupiter::IRC::Client::data_->startCAP();
	return true;
}

void Jupiter::IRC::Client::disconnect(bool stayDead)
{
	Jupiter::IRC::Client::data_->channels.emptyAndDelete();
	Jupiter::IRC::Client::data_->connectionStatus = 0;
	Jupiter::IRC::Client::data_->sock->closeSocket();
	Jupiter::IRC::Client::data_->reconnectTime = time(0) + Jupiter::IRC::Client::data_->reconnectDelay;
	Jupiter::IRC::Client::data_->dead = stayDead;
	this->OnDisconnect();
	bool ssl = Jupiter::IRC::Client::readConfigBool("SSL"_jrs);
	if (ssl != Jupiter::IRC::Client::data_->ssl)
	{
		Jupiter::IRC::Client::data_->ssl = ssl;
		if (Jupiter::IRC::Client::data_->ssl)
		{
			Jupiter::SecureTCPSocket *t = new Jupiter::SecureTCPSocket(std::move(*Jupiter::IRC::Client::data_->sock));
			if (Jupiter::IRC::Client::data_->SSLCertificate.isNotEmpty())
				t->setCertificate(Jupiter::IRC::Client::data_->SSLCertificate, Jupiter::IRC::Client::data_->SSLKey);

			delete Jupiter::IRC::Client::data_->sock;
			Jupiter::IRC::Client::data_->sock = t;
		}
		else
		{
			Jupiter::TCPSocket *t = new Jupiter::TCPSocket(std::move(*Jupiter::IRC::Client::data_->sock));
			delete Jupiter::IRC::Client::data_->sock;
			Jupiter::IRC::Client::data_->sock = t;
		}
	}
	for (size_t i = 0; i < Jupiter::plugins->size(); i++)
		Jupiter::plugins->get(i)->OnDisconnect(this);
}

void Jupiter::IRC::Client::disconnect(const Jupiter::ReadableString &message, bool stayDead)
{
	Jupiter::IRC::Client::data_->sock->send(Jupiter::StringS::Format("QUIT :%.*s" ENDL, message.size(), message.ptr()));
	Jupiter::IRC::Client::disconnect(stayDead);
}

void Jupiter::IRC::Client::reconnect()
{
	if (Jupiter::IRC::Client::data_->connectionStatus != 0) Jupiter::IRC::Client::disconnect();
	Jupiter::IRC::Client::data_->reconnectAttempts++;
	bool successConnect = Jupiter::IRC::Client::connect();
	this->OnReconnectAttempt(successConnect);
	for (size_t i = 0; i < Jupiter::plugins->size(); i++)
		Jupiter::plugins->get(i)->OnReconnectAttempt(this, successConnect);
}

int Jupiter::IRC::Client::think()
{
	int r = Jupiter::IRC::Client::primaryHandler();
	for (size_t i = 0; i < Jupiter::plugins->size(); i++)
		Jupiter::plugins->get(i)->OnThink(this);
	if (r)
	{
		if (Jupiter::IRC::Client::data_->dead == true) return r;
		if (Jupiter::IRC::Client::data_->maxReconnectAttempts < 0 || Jupiter::IRC::Client::data_->reconnectAttempts < Jupiter::IRC::Client::data_->maxReconnectAttempts)
		{
			if (!Jupiter::IRC::Client::data_->reconnectDelay || Jupiter::IRC::Client::data_->reconnectTime < time(0)) Jupiter::IRC::Client::reconnect();
		}
		else return r;
	}
	return 0;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::readConfigValue(const Jupiter::ReadableString &key, const Jupiter::ReadableString &defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::IRC::Client::Config->get(Jupiter::IRC::Client::data_->configSectionName, key);
	if (val.isNotEmpty())
		return val;
	return Jupiter::IRC::Client::Config->get("Default"_jrs, key, defaultValue);
}

bool Jupiter::IRC::Client::readConfigBool(const Jupiter::ReadableString &key, bool defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::IRC::Client::Config->get(Jupiter::IRC::Client::data_->configSectionName, key);
	if (val.isNotEmpty())
		return val.asBool();
	return Jupiter::IRC::Client::Config->getBool("Default"_jrs, key, defaultValue);
}

int Jupiter::IRC::Client::readConfigInt(const Jupiter::ReadableString &key, int defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::IRC::Client::Config->get(Jupiter::IRC::Client::data_->configSectionName, key);
	if (val.isNotEmpty())
		return val.asInt();
	return Jupiter::IRC::Client::Config->getInt("Default"_jrs, key, defaultValue);
}

long Jupiter::IRC::Client::readConfigLong(const Jupiter::ReadableString &key, long defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::IRC::Client::Config->get(Jupiter::IRC::Client::data_->configSectionName, key);
	if (val.isNotEmpty())
		return val.asInt();
	return Jupiter::IRC::Client::Config->getInt("Default"_jrs, key, defaultValue);
}

double Jupiter::IRC::Client::readConfigDouble(const Jupiter::ReadableString &key, double defaultValue) const
{
	const Jupiter::ReadableString &val = Jupiter::IRC::Client::Config->get(Jupiter::IRC::Client::data_->configSectionName, key);
	if (val.isNotEmpty())
		return val.asDouble();
	return Jupiter::IRC::Client::Config->getDouble("Default"_jrs, key, defaultValue);
}

void Jupiter::IRC::Client::writeToLogs(const Jupiter::ReadableString &message)
{
	if (Jupiter::IRC::Client::data_->logFile != nullptr)
	{
		message.println(Jupiter::IRC::Client::data_->logFile);
		fflush(Jupiter::IRC::Client::data_->logFile);
	}
}

/**
* @brief IRC Client Data Implementation
*/

void Jupiter::IRC::Client::Data::delChannel(const Jupiter::ReadableString &chan)
{
	Jupiter::IRC::Client::Channel *channel;
	for (unsigned int i = 0; i < Jupiter::IRC::Client::Data::channels.size(); i++)
	{
		channel = Jupiter::IRC::Client::Data::channels.get(i);
		if (channel->getName().equalsi(chan))
		{
			Jupiter::IRC::Client::Data::delChannel(i);
			return;
		}
	}
}

void Jupiter::IRC::Client::Data::delChannel(unsigned int index)
{
	delete Jupiter::IRC::Client::Data::channels.remove(index);
}

Jupiter::IRC::Client::User *Jupiter::IRC::Client::Data::findUser(const Jupiter::ReadableString &nick) const
{
	Jupiter::IRC::Client::User *r;
	for (unsigned int i = 0; i < Jupiter::IRC::Client::Data::users.size(); i++)
	{
		r = Jupiter::IRC::Client::Data::users.get(i);
		if (r->getNickname().equalsi(nick)) return r;
	}
	return nullptr;
}

Jupiter::IRC::Client::User *Jupiter::IRC::Client::Data::findUserOrAdd(const Jupiter::ReadableString &name)
{
	Jupiter::ReferenceString nick = Jupiter::ReferenceString::getWord(name, 0, "!");
	Jupiter::IRC::Client::User *r = Jupiter::IRC::Client::Data::findUser(nick);
	if (r == nullptr)
	{
		r = new Jupiter::IRC::Client::User();
		r->data_->nickname = nick;
		r->data_->username = Jupiter::ReferenceString::getWord(name, 1, "!@");
		r->data_->hostname = Jupiter::ReferenceString::getWord(name, 2, "!@");
		Jupiter::IRC::Client::Data::users.add(r);
	}
	return r;
}

void Jupiter::IRC::Client::Data::addNamesToChannel(unsigned int index, Jupiter::ReadableString &names)
{
	Jupiter::ReferenceString t;
	size_t offset;
	Jupiter::IRC::Client::Channel *channel = Jupiter::IRC::Client::Data::channels.get(index);
	unsigned int nameLen = names.wordCount(" ");
	for (unsigned int i = 0; i != nameLen; i++)
	{
		t = Jupiter::ReferenceString::getWord(names, i, " ");
		if (t.isNotEmpty())
		{
			offset = t.span(Jupiter::IRC::Client::Data::prefixes);
			t.shiftRight(offset);
			Jupiter::IRC::Client::User *user = Jupiter::IRC::Client::Data::findUserOrAdd(t);
			t.shiftLeft(offset);
			unsigned int userIndex = channel->addUser(user);
			while (offset > 0)
			{
				offset--;
				channel->addUserPrefix(userIndex, t[offset]);
			}
		}
	}
}

size_t Jupiter::IRC::Client::Data::addChannel(const Jupiter::ReadableString &chan)
{
	Jupiter::IRC::Client::Data::channels.add(new Channel(chan, Jupiter::IRC::Client::Data::iFace));
	return Jupiter::IRC::Client::Data::channels.size() - 1;
}

bool Jupiter::IRC::Client::Data::startCAP()
{
	Jupiter::IRC::Client::Data::connectionStatus = 2;
	return Jupiter::IRC::Client::Data::sock->send("CAP LS" ENDL, 8) > 0;
}
//Jupiter::ReferenceString &
bool Jupiter::IRC::Client::Data::registerClient()
{
	bool r = true;
	const char *localHostname = Jupiter::Socket::getLocalHostname();
	Jupiter::StringS messageToSend;
	messageToSend.format("USER %.*s %s %.*s :%.*s" ENDL, Jupiter::IRC::Client::Data::nickname.size(), Jupiter::IRC::Client::Data::nickname.ptr(), localHostname, Jupiter::IRC::Client::Data::serverHostname.size(), Jupiter::IRC::Client::Data::serverHostname.ptr(), Jupiter::IRC::Client::Data::realName.size(), Jupiter::IRC::Client::Data::realName.ptr());
	if (Jupiter::IRC::Client::Data::sock->send(messageToSend) <= 0) r = false;
	messageToSend.format("NICK %.*s" ENDL, Jupiter::IRC::Client::Data::nickname.size(), Jupiter::IRC::Client::Data::nickname.ptr());
	if (Jupiter::IRC::Client::Data::sock->send(messageToSend) <= 0) r = false;
	Jupiter::IRC::Client::Data::connectionStatus = 3;
	return r;
}

/**
* User Implementation
*/

Jupiter::IRC::Client::User::User()
{
	Jupiter::IRC::Client::User::data_ = new Jupiter::IRC::Client::User::Data();
}

Jupiter::IRC::Client::User::~User()
{
	delete Jupiter::IRC::Client::User::data_;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::User::getNickname() const
{
	return Jupiter::IRC::Client::User::data_->nickname;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::User::getUsername() const
{
	return Jupiter::IRC::Client::User::data_->username;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::User::getHostname() const
{
	return Jupiter::IRC::Client::User::data_->hostname;
}

size_t Jupiter::IRC::Client::User::getChannelCount() const
{
	return Jupiter::IRC::Client::User::data_->channelCount;
}

/**
* Channel Implementation
*/

Jupiter::IRC::Client::Channel::Channel(const Jupiter::ReadableString &channelName, Jupiter::IRC::Client *iFace)
{
	Jupiter::IRC::Client::Channel::data_ = new Jupiter::IRC::Client::Channel::Data();
	Jupiter::IRC::Client::Channel::data_->channel = channelName;
	Jupiter::IRC::Client::Channel::data_->serverPrefixSetPtr = iFace->getPrefixes();

	Jupiter::String key = "Channel.";
	size_t offset = key.aformat("%.*s.Type", channelName.size(), channelName.ptr());

	iFace->readConfigValue(key);
	Jupiter::ReferenceString value = iFace->readConfigValue(key);
	if (value.isNotEmpty())
	{
		Jupiter::IRC::Client::Channel::data_->type = value.asInt();
		return;
	}

	// TODO: Make this more efficient -- consider moving responsibility elsewhere.
	for (unsigned int i = 1;; i++)
	{
		key.truncate(offset);
		offset = key.aformat("%u", i);
		value.set(iFace->readConfigValue(key));

		if (value.isEmpty()) // No more channels in the list.
			break;

		if (value.getWord(0, WHITESPACE).equalsi(channelName)) // This is our channel.
		{
			offset += key.concat(".Type");
			value.set(iFace->readConfigValue(key));
			if (value.isNotEmpty())
				Jupiter::IRC::Client::Channel::data_->type = value.asInt();
			else if (iFace->getDefaultChanType() < 0)
				Jupiter::IRC::Client::Channel::data_->type = 0;
			else
				Jupiter::IRC::Client::Channel::data_->type = iFace->getDefaultChanType();
			return;
		}
	}

	Jupiter::IRC::Client::Channel::data_->type = iFace->getDefaultChanType();
}

Jupiter::IRC::Client::Channel::~Channel()
{
	Jupiter::IRC::Client::Channel::data_->users.emptyAndDelete();
	delete Jupiter::IRC::Client::Channel::data_;
}

unsigned int Jupiter::IRC::Client::Channel::addUser(Jupiter::IRC::Client::User *user)
{
	Jupiter::IRC::Client::Channel::User *localUser = new Jupiter::IRC::Client::Channel::User();
	localUser->data_->user = user;
	user->data_->channelCount++;
	Jupiter::IRC::Client::Channel::data_->users.add(localUser);
	return Jupiter::IRC::Client::Channel::data_->users.size() - 1;
}

unsigned int Jupiter::IRC::Client::Channel::addUser(Jupiter::IRC::Client::User *user, const char prefix)
{
	unsigned int r = Jupiter::IRC::Client::Channel::addUser(user);
	Jupiter::IRC::Client::Channel::data_->users.get(r)->data_->prefixes = prefix;
	return r;
}

void Jupiter::IRC::Client::Channel::delUser(const Jupiter::ReadableString &nickname)
{
	int index = getUserIndex(nickname);
	if (index >= 0) Jupiter::IRC::Client::Channel::delUser(index);
}

void Jupiter::IRC::Client::Channel::delUser(size_t index)
{
	if (index < Jupiter::IRC::Client::Channel::data_->users.size())
		delete Jupiter::IRC::Client::Channel::data_->users.remove(index);
}

void Jupiter::IRC::Client::Channel::addUserPrefix(size_t index, char prefix)
{
	if (index < Jupiter::IRC::Client::Channel::data_->users.size())
	{
		if (Jupiter::IRC::Client::Channel::data_->users.get(index)->data_->prefixes.contains(prefix) == false)
		{
			Jupiter::IRC::Client::Channel::data_->users.get(index)->data_->prefixes += prefix;
		}
	}
}

void Jupiter::IRC::Client::Channel::addUserPrefix(const Jupiter::ReadableString &user, char prefix)
{
	int i = Jupiter::IRC::Client::Channel::getUserIndex(user);
	if (i >= 0) Jupiter::IRC::Client::Channel::addUserPrefix(i, prefix);
}

void Jupiter::IRC::Client::Channel::delUserPrefix(unsigned int index, char prefix)
{
	if (index < Jupiter::IRC::Client::Channel::data_->users.size()) Jupiter::IRC::Client::Channel::data_->users.get(index)->data_->prefixes.remove(prefix);
}

void Jupiter::IRC::Client::Channel::delUserPrefix(const Jupiter::ReadableString &user, char prefix)
{
	int i = Jupiter::IRC::Client::Channel::getUserIndex(user);
	if (i >= 0) Jupiter::IRC::Client::Channel::delUserPrefix(i, prefix);
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::getName() const
{
	return Jupiter::IRC::Client::Channel::data_->channel;
}

Jupiter::IRC::Client::Channel::User *Jupiter::IRC::Client::Channel::getUser(unsigned int index) const
{
	Jupiter::ArrayList<Jupiter::IRC::Client::Channel::User> &users = Jupiter::IRC::Client::Channel::data_->users;
	if (index < users.size()) return users.get(index);
	return nullptr;
}

Jupiter::IRC::Client::Channel::User *Jupiter::IRC::Client::Channel::getUser(const Jupiter::ReadableString &nickname) const
{
	Jupiter::ArrayList<Jupiter::IRC::Client::Channel::User> &users = Jupiter::IRC::Client::Channel::data_->users;
	Jupiter::IRC::Client::Channel::User *user;
	for (unsigned int i = 0; i < users.size(); i++)
	{
		user = users.get(i);
		if (user->data_->user->getNickname().equalsi(nickname)) return user;
	}
	return nullptr;
}

int Jupiter::IRC::Client::Channel::getUserIndex(const Jupiter::ReadableString &user) const
{
	for (unsigned int i = 0; i < Jupiter::IRC::Client::Channel::data_->users.size(); i++)
		if (Jupiter::IRC::Client::Channel::data_->users[i] && Jupiter::IRC::Client::Channel::data_->users[i]->data_->user->getNickname().equalsi(user))
			return i;
	return -1;
}

int Jupiter::IRC::Client::Channel::getUserIndexByPartName(const Jupiter::ReadableString &user) const
{
	for (unsigned int i = 0; i < Jupiter::IRC::Client::Channel::data_->users.size(); i++)
		if (Jupiter::IRC::Client::Channel::data_->users[i] && Jupiter::IRC::Client::Channel::data_->users[i]->data_->user->getNickname().find(user) != Jupiter::INVALID_INDEX)
			return i;
	return -1;
}

char Jupiter::IRC::Client::Channel::getUserPrefix(unsigned int index) const
{
	if (index < Jupiter::IRC::Client::Channel::data_->users.size())
		for (unsigned char i = 0; Jupiter::IRC::Client::Channel::data_->serverPrefixSetPtr[i]; i++)
			if (Jupiter::IRC::Client::Channel::data_->users.get(index)->data_->prefixes.contains(Jupiter::IRC::Client::Channel::data_->serverPrefixSetPtr[i]))
				return Jupiter::IRC::Client::Channel::data_->serverPrefixSetPtr[i];
	return 0;
}

char Jupiter::IRC::Client::Channel::getUserPrefix(const Jupiter::ReadableString &user) const
{
	int i = Jupiter::IRC::Client::Channel::getUserIndex(user);
	if (i >= 0) return Jupiter::IRC::Client::Channel::getUserPrefix(i);
	return 0;
}

unsigned int Jupiter::IRC::Client::Channel::getUserCount() const
{
	return Jupiter::IRC::Client::Channel::data_->users.size();
}

int Jupiter::IRC::Client::Channel::getType() const
{
	return Jupiter::IRC::Client::Channel::data_->type;
}

void Jupiter::IRC::Client::Channel::setType(int iType)
{
	Jupiter::IRC::Client::Channel::data_->type = iType;
}

/**
* Channel User Implementation
*/

Jupiter::IRC::Client::Channel::User::User()
{
	Jupiter::IRC::Client::Channel::User::data_ = new Jupiter::IRC::Client::Channel::User::Data();
}

Jupiter::IRC::Client::Channel::User::~User()
{
	delete Jupiter::IRC::Client::Channel::User::data_;
}

Jupiter::IRC::Client::User *Jupiter::IRC::Client::Channel::User::getUser() const
{
	return Jupiter::IRC::Client::Channel::User::data_->user;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::User::getPrefixes() const
{
	return Jupiter::IRC::Client::Channel::User::data_->prefixes;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::User::getNickname() const
{
	return Jupiter::IRC::Client::Channel::User::data_->user->getNickname();
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::User::getUsername() const
{
	return Jupiter::IRC::Client::Channel::User::data_->user->getUsername();
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::User::getHostname() const
{
	return Jupiter::IRC::Client::Channel::User::data_->user->getHostname();
}

unsigned int Jupiter::IRC::Client::Channel::User::getChannelCount() const
{
	return Jupiter::IRC::Client::Channel::User::data_->user->getChannelCount();
}