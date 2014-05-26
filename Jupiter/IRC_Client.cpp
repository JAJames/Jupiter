/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
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
#include "Plugin.h"
#include "Base64.h"

//#define SHORT_IRC_MACROS
#include "IRC_Numerics.h"

#if defined _WIN32
#include <WinSock2.h>
#else // _WIN32
#include <unistd.h>
#endif // _WIN32

Jupiter::INIFile _Config;
Jupiter::INIFile *Jupiter::IRC::Client::Config = &_Config;

template class JUPITER_API Jupiter::CString_Strict<char>;
template class JUPITER_API Jupiter::ArrayList<Jupiter::IRC::Client::User>;
template class JUPITER_API Jupiter::ArrayList<Jupiter::IRC::Client::Channel>;

struct JUPITER_API Jupiter::IRC::Client::Data
{
	Jupiter::IRC::Client *iFace;
	Jupiter::Socket *sock;
	bool ssl;
	Jupiter::CStringS SSLCertificate;
	Jupiter::CStringS SSLKey;
	Jupiter::CStringS saslAccount;
	Jupiter::CStringS saslPass;
	int connectionStatus;
	Jupiter::CStringS configSectionName;
	Jupiter::CStringS logFileName;
	Jupiter::CStringS buffer;
	unsigned short serverPort;
	Jupiter::CStringS serverHostname;
	Jupiter::CStringS serverName;
	Jupiter::CStringS nickname;
	Jupiter::CStringS realName;
	Jupiter::CStringS prefixModes = "ov";
	Jupiter::CStringS prefixes = "@+";
	Jupiter::CStringS chanTypes = "#";
	Jupiter::CStringS modeA = "b";
	Jupiter::CStringS modeB = "k";
	Jupiter::CStringS modeC = "l";
	Jupiter::CStringS modeD = "psitnm";
	Jupiter::ArrayList<Channel> channels;
	void delChannel(unsigned int index);
	void delChannel(const char *chan);
	void addNamesToChannel(unsigned int channelIndex, Jupiter::CStringS &names);
	unsigned int addChannel(const char *chan);
	bool joinOnKick;
	Jupiter::CStringS autoPartMessage;
	time_t reconnectDelay;
	time_t reconnectTime;
	int maxReconnectAttempts;
	int reconnectAttempts;
	FILE *printOutput;
	FILE *logFile;
	short dChanType;
	bool dead = false;
	Jupiter::ArrayList<Jupiter::IRC::Client::User> users;

	bool startCAP();
	bool registerClient();
	Data(Jupiter::IRC::Client *interFace);
	Jupiter::IRC::Client::User *findUser(const char *nick) const;
	Jupiter::IRC::Client::User *findUserOrAdd(Jupiter::CStringS &nick);
};

Jupiter::IRC::Client::Data::Data(Jupiter::IRC::Client *interFace)
{
	Jupiter::IRC::Client::Data::iFace = interFace;
}

struct Jupiter::IRC::Client::User::Data
{
	unsigned int channelCount = 0;
	Jupiter::CStringS nickname;
	Jupiter::CStringS username;
	Jupiter::CStringS hostname;
};

struct Jupiter::IRC::Client::Channel::User::Data
{
	Jupiter::IRC::Client::User *user;
	Jupiter::CStringS prefixes;
};

struct Jupiter::IRC::Client::Channel::Data
{
	Jupiter::CStringS channel;
	Jupiter::ArrayList<Jupiter::IRC::Client::Channel::User> users;
	const char *serverPrefixSetPtr;
	int type;
	bool isAddingNames;
};

Jupiter::IRC::Client::Client(const char *configSection)
{
	Jupiter::IRC::Client::data_ = new Jupiter::IRC::Client::Data(this);
	if (Jupiter::IRC::Client::Config == nullptr)
	{
		Jupiter::IRC::Client::Config = new INIFile();
		Jupiter::IRC::Client::Config->readFile(CONFIG_INI);
	}
	Jupiter::IRC::Client::data_->configSectionName = configSection;
	Jupiter::IRC::Client::data_->serverHostname = Jupiter::IRC::Client::readConfigValue("Hostname", "irc.tibitek.com");
	
	Jupiter::IRC::Client::data_->logFileName = Jupiter::IRC::Client::readConfigValue("LogFile");
	Jupiter::IRC::Client::data_->nickname = Jupiter::IRC::Client::readConfigValue("Nick", "Jupiter");

	Jupiter::IRC::Client::data_->realName = Jupiter::IRC::Client::readConfigValue("Realname", "Jupiter IRC Client");

	Jupiter::IRC::Client::data_->saslPass = Jupiter::IRC::Client::readConfigValue("SASL.Password");
	if (Jupiter::IRC::Client::data_->saslPass.size() == 0) Jupiter::IRC::Client::data_->saslPass = Jupiter::IRC::Client::readConfigValue("SASL.Pass");
	
	Jupiter::IRC::Client::data_->saslAccount = Jupiter::IRC::Client::readConfigValue("SASL.Account");
	if (Jupiter::IRC::Client::data_->saslAccount.size() == 0) Jupiter::IRC::Client::data_->saslAccount = Jupiter::IRC::Client::data_->nickname;

	Jupiter::IRC::Client::data_->autoPartMessage = Jupiter::IRC::Client::readConfigValue("AutoPartMessage");

	Jupiter::IRC::Client::data_->ssl = Jupiter::IRC::Client::readConfigBool("SSL");
	Jupiter::IRC::Client::data_->SSLCertificate = Jupiter::IRC::Client::readConfigValue("Certificate");
	Jupiter::IRC::Client::data_->SSLKey = Jupiter::IRC::Client::readConfigValue("Key");
	if (Jupiter::IRC::Client::data_->SSLCertificate.size() == 0)
	{
		Jupiter::IRC::Client::data_->SSLCertificate = Jupiter::IRC::Client::readConfigValue("Cert");
		if (Jupiter::IRC::Client::data_->SSLCertificate.size() == 0) Jupiter::IRC::Client::data_->SSLCertificate = Jupiter::IRC::Client::data_->SSLKey;
	}
	if (Jupiter::IRC::Client::data_->SSLKey.size() == 0) Jupiter::IRC::Client::data_->SSLKey = Jupiter::IRC::Client::data_->SSLCertificate;

	Jupiter::IRC::Client::data_->joinOnKick = Jupiter::IRC::Client::readConfigBool("AutoJoinOnKick");
	Jupiter::IRC::Client::data_->reconnectDelay = Jupiter::IRC::Client::readConfigInt("AutoReconnectDelay");
	Jupiter::IRC::Client::data_->maxReconnectAttempts = Jupiter::IRC::Client::readConfigInt("MaxReconnectAttempts");
	Jupiter::IRC::Client::data_->serverPort = (unsigned short) Jupiter::IRC::Client::readConfigInt("Port");
	if (Jupiter::IRC::Client::data_->serverPort == 0)
	{
		if (Jupiter::IRC::Client::data_->ssl) Jupiter::IRC::Client::data_->serverPort = 994;
		else Jupiter::IRC::Client::data_->serverPort = 194;
	}
	Jupiter::IRC::Client::data_->dChanType = (short) Jupiter::IRC::Client::readConfigInt("Channel.Type");

	if (Jupiter::IRC::Client::readConfigBool("PrintOutput", true)) Jupiter::IRC::Client::data_->printOutput = stdout;
	else Jupiter::IRC::Client::data_->printOutput = nullptr;
	if (Jupiter::IRC::Client::data_->logFileName.size() != 0) Jupiter::IRC::Client::data_->logFile = fopen(Jupiter::IRC::Client::data_->logFileName.c_str(), "a+b");
	else Jupiter::IRC::Client::data_->logFile = nullptr;

	if (Jupiter::IRC::Client::data_->ssl)
	{
		Jupiter::SecureTCPSocket *t = new Jupiter::SecureTCPSocket();
		if (Jupiter::IRC::Client::data_->SSLCertificate.size() != 0)
		{
			t->setCertificate(Jupiter::IRC::Client::data_->SSLCertificate.c_str(), Jupiter::IRC::Client::data_->SSLKey.c_str());
		}
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

void Jupiter::IRC::Client::OnRaw(const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnNumeric(long int, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnError(const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnChat(const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnNotice(const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnServerNotice(const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnCTCP(const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnAction(const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnInvite(const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnJoin(const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnPart(const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnNick(const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnKick(const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnQuit(const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

void Jupiter::IRC::Client::OnMode(const Jupiter::StringType &, const Jupiter::StringType &, const Jupiter::StringType &)
{
	return;
}

const Jupiter::StringType &Jupiter::IRC::Client::getConfigSection() const
{
	return Jupiter::IRC::Client::data_->configSectionName;
}

const Jupiter::StringType &Jupiter::IRC::Client::getLogFile() const
{
	return Jupiter::IRC::Client::data_->logFileName;
}

const Jupiter::StringType &Jupiter::IRC::Client::getPrefixes() const
{
	return Jupiter::IRC::Client::data_->prefixes;
}

const Jupiter::StringType &Jupiter::IRC::Client::getNickname() const
{
	return Jupiter::IRC::Client::data_->nickname;
}

const Jupiter::StringType &Jupiter::IRC::Client::getRealname() const
{
	return Jupiter::IRC::Client::data_->realName;
}

const Jupiter::StringType &Jupiter::IRC::Client::getServerName() const
{
	return Jupiter::IRC::Client::data_->serverName;
}

const Jupiter::StringType &Jupiter::IRC::Client::getServerHostname() const
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

short Jupiter::IRC::Client::getDefaultChanType() const
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

inline Jupiter::CStringS getSender(Jupiter::CStringS &line)
{
	return line.getWord(0, ":! ");
}

int Jupiter::IRC::Client::getAccessLevel(const char *chan, const char *nick) const
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
			return Jupiter::IRC::Client::data_->prefixes.size() - findSymbol(Jupiter::IRC::Client::data_->prefixes.c_str(), prefix, 0);
		}
		i++;
	}
	return 0;
}

void Jupiter::IRC::Client::send(const char *rawMessage)
{
	char *sentMessage = new char[strlen(rawMessage) + 3];
	sprintf(sentMessage, "%s" ENDL, rawMessage);
	Jupiter::IRC::Client::data_->sock->send(sentMessage);
}

Jupiter::IRC::Client::User *Jupiter::IRC::Client::getUser(unsigned int index) const
{
	return Jupiter::IRC::Client::data_->users.get(index);
}

Jupiter::IRC::Client::User *Jupiter::IRC::Client::getUser(const char *nickname) const
{
	Jupiter::IRC::Client::User *r;
	for (unsigned int i = 0; i < Jupiter::IRC::Client::data_->users.size(); i++)
	{
		r = Jupiter::IRC::Client::data_->users.get(i);
		if (r->getNickname().equalsi(nickname)) return r;
	}
	return nullptr;
}

int Jupiter::IRC::Client::getUserIndex(const char *nickname) const
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

Jupiter::IRC::Client::Channel *Jupiter::IRC::Client::getChannel(const char *chanName) const
{
	unsigned int index = Jupiter::IRC::Client::getChannelIndex(chanName);
	if (index < 0) return nullptr;
	return Jupiter::IRC::Client::getChannel(index);
}

const Jupiter::StringType &Jupiter::IRC::Client::getChannelName(unsigned int index) const
{
	return Jupiter::IRC::Client::data_->channels.get(index)->getName();
}

int Jupiter::IRC::Client::getChannelIndex(const char *chanName) const
{
	for (unsigned int i = 0; i < Jupiter::IRC::Client::data_->channels.size(); i++) if (Jupiter::IRC::Client::data_->channels.get(i)->getName().equalsi(chanName)) return i;
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

void Jupiter::IRC::Client::joinChannel(const char *channel)
{
	char *msg = new char[strlen(channel) + 8];
	sprintf(msg, "JOIN %s" ENDL, channel);
	Jupiter::IRC::Client::data_->sock->send(msg);
	delete[] msg;
}

void Jupiter::IRC::Client::joinChannel(const char *channel, const char *password)
{
	char *msg = new char[strlen(channel) + strlen(password) + 9];
	sprintf(msg, "JOIN %s %s" ENDL, channel, password);
	Jupiter::IRC::Client::data_->sock->send(msg);
	delete[] msg;
}

void Jupiter::IRC::Client::partChannel(const char *channel)
{
	char *msg = new char[strlen(channel) + 8];
	sprintf(msg, "PART %s" ENDL, channel);
	Jupiter::IRC::Client::data_->sock->send(msg);
	delete[] msg;
	int index = Jupiter::IRC::Client::getChannelIndex(channel);
	if (index >= 0) Jupiter::IRC::Client::data_->channels.get(index)->setType(-2);
}

void Jupiter::IRC::Client::partChannel(const char *channel, const char *message)
{
	char *msg = new char[strlen(channel) + strlen(message) + 10];
	sprintf(msg, "PART %s :%s" ENDL, channel, message);
	Jupiter::IRC::Client::data_->sock->send(msg);
	delete[] msg;
	int index = Jupiter::IRC::Client::getChannelIndex(channel);
	if (index >= 0) Jupiter::IRC::Client::data_->channels.get(index)->setType(-2);
}

void Jupiter::IRC::Client::sendMessage(const char *dest, const char *message)
{
	char *msg = new char[strlen(dest) + strlen(message) + 13];
	sprintf(msg, "PRIVMSG %s :%s" ENDL, dest, message);
	Jupiter::IRC::Client::data_->sock->send(msg);
	delete[] msg;
}

void Jupiter::IRC::Client::sendNotice(const char *dest, const char *message)
{
	char *msg = new char[strlen(dest) + strlen(message) + 12];
	sprintf(msg, "NOTICE %s :%s" ENDL, dest, message);
	Jupiter::IRC::Client::data_->sock->send(msg);
	delete[] msg;
}

unsigned int Jupiter::IRC::Client::messageChannels(short type, const char *message)
{
	unsigned int total = 0;
	Jupiter::IRC::Client::Channel *channel;
	for (unsigned int i = 0; i < Jupiter::IRC::Client::data_->channels.size(); i++)
	{
		channel = Jupiter::IRC::Client::data_->channels.get(i);
		if (channel->getType() == type)
		{
			Jupiter::IRC::Client::sendMessage(channel->getName().c_str(), message);
			total++;
		}
	}
	return total;
}

unsigned int Jupiter::IRC::Client::messageChannels(const char *message)
{
	unsigned int total = 0;
	Jupiter::IRC::Client::Channel *channel;
	for (unsigned int i = 0; i < Jupiter::IRC::Client::data_->channels.size(); i++)
	{
		channel = Jupiter::IRC::Client::data_->channels.get(i);
		if (channel->getType() >= 0)
		{
			Jupiter::IRC::Client::sendMessage(channel->getName().c_str(), message);
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
		unsigned int totalLines = countLines(Jupiter::IRC::Client::data_->buffer.c_str());
		for (unsigned int currentLine = 0; currentLine < totalLines; currentLine++)
		{
			Jupiter::CStringS buff = Jupiter::IRC::Client::data_->buffer.getWord(currentLine, ENDL);
			if (buff.size() != 0)
			{
				Jupiter::IRC::Client::writeToLogs(buff.c_str());
				if (Jupiter::IRC::Client::data_->printOutput != nullptr)
				{
					fputs(buff.c_str(), Jupiter::IRC::Client::data_->printOutput);
					fputs(ENDL, Jupiter::IRC::Client::data_->printOutput);
				}
				Jupiter::CStringS w1 = buff.getWord(0, WHITESPACE);
				if (w1.size() != 0)
				{
					Jupiter::CStringS w2 = buff.getWord(1, WHITESPACE);
					long int numeric = strtol(w2.c_str(), nullptr, 10);
					if (w1[0] == ':') //Messages
					{
						if (w2.size() != 0)
						{
							switch (numeric) // Numerics that don't rely on a specific connectionStatus.
							{
							case IRC_RPL_BOUNCE: // 010
							{
								Jupiter::CStringS portToken = buff.getWord(4, " ");
								unsigned short port;
								if (portToken[0] == '+') // This is most likely not used anywhere.
								{
									port = (unsigned short)strtoul(portToken.c_str() + 1, nullptr, 10);
									if (Jupiter::IRC::Client::data_->ssl == false)
									{
										Jupiter::IRC::Client::data_->ssl = true;
										delete Jupiter::IRC::Client::data_->sock;
										Jupiter::IRC::Client::data_->sock = new Jupiter::SecureTCPSocket();
									}
								}
								else
								{
									port = (unsigned short)strtoul(portToken.c_str(), nullptr, 10);
									if (Jupiter::IRC::Client::data_->ssl == true)
									{
										Jupiter::IRC::Client::data_->ssl = false;
										delete Jupiter::IRC::Client::data_->sock;
										Jupiter::IRC::Client::data_->sock = new Jupiter::TCPSocket();
									}
								}
								if (port != 0) // Don't default -- could be non-compliant input.
								{
									Jupiter::IRC::Client::data_->serverHostname = buff.getWord(3, WHITESPACE);
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
										Jupiter::CStringS portToken = buff.getWord(6, " ");
										unsigned short bouncePort = (unsigned short) atoi(buff.getWord(6, " ").c_str());

										if (portToken[0] == '+') // This is almost certainly not used anywhere.
										{
											bouncePort = (unsigned short)strtoul(portToken.c_str() + 1, nullptr, 10);
											if (Jupiter::IRC::Client::data_->ssl == false)
											{
												Jupiter::IRC::Client::data_->ssl = true;
												delete Jupiter::IRC::Client::data_->sock;
												Jupiter::IRC::Client::data_->sock = new Jupiter::SecureTCPSocket();
											}
										}
										else
										{
											bouncePort = (unsigned short)strtoul(portToken.c_str(), nullptr, 10);
											if (Jupiter::IRC::Client::data_->ssl == true)
											{
												Jupiter::IRC::Client::data_->ssl = false;
												delete Jupiter::IRC::Client::data_->sock;
												Jupiter::IRC::Client::data_->sock = new Jupiter::TCPSocket();
											}
										}
										if (bouncePort != 0)
										{
											Jupiter::IRC::Client::data_->serverHostname = buff.getWord(4, " ");
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
									Jupiter::CStringS command = buff.getWord(2, " ");
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
									if (Jupiter::IRC::Client::data_->SSLCertificate.size() != 0)
									{
										t->setCertificate(Jupiter::IRC::Client::data_->SSLCertificate.c_str(), Jupiter::IRC::Client::data_->SSLKey.c_str());
									}

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
										Jupiter::CStringS w4 = buff.getWord(3, WHITESPACE);
										if (w4.equals("LS"))
										{
											Jupiter::CStringS listParams = buff.gotoWord(4, WHITESPACE);
											if (listParams[0] == ':') listParams.shiftRight(1);
											unsigned int len = listParams.wordCount(WHITESPACE);
											Jupiter::CStringS curr;
											Jupiter::CStringL req = "CAP REQ :";
											bool sasl = false;
											for (unsigned int i = 0; i < len; i++)
											{
												curr = listParams.getWord(i, WHITESPACE);
												if (curr.equalsi("multi-prefix")) req += "multi-prefix ";
												else if (curr.equalsi("userhost-in-names")) req += "userhost-in-names ";
												else if (curr.equalsi("sasl"))
												{
													if (Jupiter::IRC::Client::data_->saslPass.size() != 0)
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
												Jupiter::IRC::Client::data_->sock->send(req.c_str());
												if (sasl)
												{
													Jupiter::IRC::Client::data_->sock->send("AUTHENTICATE PLAIN" ENDL);
												}
											}
											if (!sasl)
											{
												Jupiter::IRC::Client::data_->sock->send("CAP END" ENDL);
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
									Jupiter::IRC::Client::data_->serverName = buff.getWord(3, " ");
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
									Jupiter::CStringS altNick = Jupiter::IRC::Client::readConfigValue("AltNick", "Jupiter");
									Jupiter::CStringS configNick = Jupiter::IRC::Client::readConfigValue("Nick", "Jupiter");
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
										if (Jupiter::IRC::Client::data_->nickname.size() != 0)
										{
											char *messageToSend = new char[Jupiter::IRC::Client::data_->nickname.size() + 8];
											sprintf(messageToSend, "NICK %s" ENDL, Jupiter::IRC::Client::data_->nickname.c_str());
											Jupiter::IRC::Client::data_->sock->send(messageToSend);
											delete[] messageToSend;
										}
									}
									// Note: Add a series of contains() functions to String_Type.
									else //if (stristr(Jupiter::IRC::Client::data_->nickname.c_str(), configNick.c_str()) == Jupiter::IRC::Client::data_->nickname.c_str()) // We're already adding numbers.
									{
										if (completelyBadNick == false) // If this nick is invalid, adding numbers won't help.
										{
											if (Jupiter::IRC::Client::data_->nickname.size() > configNick.size())
											{
												int n = atoi(Jupiter::IRC::Client::data_->nickname.c_str() + configNick.size()) + 1;
												Jupiter::IRC::Client::data_->nickname.format("%s%d", configNick.c_str(), n);
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
									const char *ptr = strstr(buff.c_str(), "PREFIX=(");
									if (ptr != nullptr)
									{
										ptr += 8;
										int poff = findSymbol(ptr, ')', 0);
										Jupiter::IRC::Client::data_->prefixModes = Jupiter::CStringS::getWord(ptr, 0, ")");
										ptr += poff + 1;
										Jupiter::IRC::Client::data_->prefixes = Jupiter::CStringS::getWord(ptr, 0, " " ENDL);
										ptr = nullptr;
									}
									ptr = strstr(buff.c_str(), "CHANMODES=");
									if (ptr != nullptr)
									{
										ptr += 10;
										int pos = findSymbol(ptr, ',', 0);
										if (pos >= 0)
										{
											Jupiter::IRC::Client::data_->modeA = Jupiter::CStringS::getWord(ptr, 0, ", ");
											ptr += pos + 1;
											pos = findSymbol(ptr, ',', 0);
											if (pos >= 0)
											{
												Jupiter::IRC::Client::data_->modeB = Jupiter::CStringS::getWord(ptr, 0, ", ");
												ptr += pos + 1;
												pos = findSymbol(ptr, ',', 0);
												if (pos >= 0)
												{
													Jupiter::IRC::Client::data_->modeC = Jupiter::CStringS::getWord(ptr, 0, ", ");
													ptr += pos + 1;
													pos = strcspn(ptr, " ");
													if (pos >= 0) Jupiter::IRC::Client::data_->modeD = Jupiter::CStringS::getWord(ptr, 0, ", ");
												}
											}
										}
									}
									ptr = strstr(buff.c_str(), "CHANTYPES=");
									if (ptr != nullptr)
									{
										ptr += 10;
										Jupiter::IRC::Client::data_->chanTypes = Jupiter::CStringS::getWord(ptr, 0, " ");
									}
								}
									break;
								case IRC_RPL_LUSERCLIENT: // 251
								{
									Jupiter::CStringL key = "RawData.";
									unsigned int offset;

									Jupiter::CStringS tVal;
									unsigned int i = 1;
									do
									{
										offset = key.aformat("%u", i);
										tVal = Jupiter::IRC::Client::readConfigValue(key.c_str());
										if (tVal.size() == 0) break;
										key -= offset;
										Jupiter::IRC::Client::send(tVal.c_str());
										i++;
									} while (1);
									key = "Channel.";
									i = 1;
									do
									{
										offset = key.aformat("%u", i);
										tVal = Jupiter::IRC::Client::readConfigValue(key.c_str());
										if (tVal.size() == 0) break;
										key -= offset;
										Jupiter::IRC::Client::joinChannel(tVal.c_str());
										i++;
									} while (1);

									Jupiter::IRC::Client::data_->connectionStatus = 5;
									Jupiter::IRC::Client::data_->reconnectAttempts = 0;
									this->OnConnect();
									for (i = 0; i < Jupiter::plugins->size(); i++) Jupiter::plugins->get(i)->OnConnect(this);
								}
									break;
								}
								break;

							default: // Post-registration.
								if (streqli(w2.c_str(), "PRIVMSG"))
								{
									Jupiter::CStringS chan = buff.getWord(2, WHITESPACE);
									if (chan.size() != 0)
									{
										Jupiter::CStringS nick = getSender(buff);
										if (nick.size() != 0)
										{
											const char *premessage = buff.c_str() + findSymbol(buff.c_str(), ':', 1) + 1;
											if (premessage[0] == '\001') //CTCP (ACTIONs are included)
											{
												Jupiter::CStringS rawmessage = premessage + 1;
												Jupiter::CStringS command = rawmessage.getWord(0, WHITESPACE);
												if (command[command.size() - 1] == IRC::CTCP) command.truncate(1);
												Jupiter::CStringS message = rawmessage.substring(findSymbol(rawmessage.c_str(), ' ', 0) + 1, findSymbol(rawmessage.c_str(), IRC::CTCP, 0));
												if (message[message.size() - 1] == IRC::CTCP) message.truncate(1);
												
												if (command.equals("ACTION"))
												{
													this->OnAction(chan, nick, message);
													for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnAction(this, chan, nick, message);
												}
												else
												{
													Jupiter::CStringL response = "NOTICE ";
													response += nick;
													response += " :" IRCCTCP;
													response += command;
													response += ' ';
													if (command.equals("PING")) response += message;
													else if (command.equals("VERSION")) response += Jupiter::version;
													else if (command.equals("FINGER")) response += "Oh, yeah, a little to the left.";
													else if (command.equals("SOURCE")) response += "irc.tibitek.com";
													else if (command.equals("USERINFO")) response += "Hey, I'm Jupiter! If you have questions, ask Justin! (irc.tibitek.com)";
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
													Jupiter::IRC::Client::data_->sock->send(response.c_str());
													this->OnCTCP(chan, nick, command, message);
													for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnCTCP(this, chan, nick, message);
												}
											}
											else
											{
												Jupiter::CStringS message = premessage;
												this->OnChat(chan, nick, message);
												for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnChat(this, chan, nick, message);
											}
										}
									}
								}
								else if (streqli(w2.c_str(), "NOTICE"))
								{
									CStringS chan = buff.getWord(2, WHITESPACE);
									if (chan.size() != 0)
									{
										int pos = findSymbol(buff.c_str(), '!', 0);
										auto message = buff.substring(findSymbol(buff.c_str(), ':', 1) + 1, buff.size());
										if (pos >= 0 && strcspn(buff.c_str(), "!") < strcspn(buff.c_str(), " "))
										{
											auto nick = buff.substring(1, pos);
											this->OnNotice(chan, nick, message);
											for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnNotice(this, chan, nick, message);
										}
										else
										{
											auto sender = getSender(buff);
											if (sender.size() != 0)
											{
												this->OnServerNotice(chan, sender, message);
												for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnServerNotice(this, chan, sender, message);
											}
										}
									}
								}
								else if (streqli(w2.c_str(), "NICK"))
								{
									auto nick = getSender(buff);
									const char *tnewnick = buff.c_str() + findSymbol(buff.c_str(), ' ', 1) + 1;
									if (*tnewnick == ':') tnewnick++;
									Jupiter::CStringS newnick = tnewnick;
									if (nick.equalsi(Jupiter::IRC::Client::data_->nickname))
									{
										Jupiter::IRC::Client::data_->nickname = newnick;
									}
									Jupiter::IRC::Client::User *user = Jupiter::IRC::Client::data_->findUser(nick.c_str());
									if (user != nullptr)
									{
										user->data_->nickname = newnick;
										this->OnNick(nick, newnick);
									}
									for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnNick(this, nick, newnick);
								}
								else if (streqli(w2.c_str(), "JOIN"))
								{
									auto nick = getSender(buff);
									CStringS chan = buff.getWord(2, WHITESPACE);
									if (chan[0] == ':') chan.shiftRight(1);
									int i = Jupiter::IRC::Client::getChannelIndex(chan.c_str());
									if (Jupiter::IRC::Client::data_->nickname.equalsi(nick))
									{
										if (i >= 0) Jupiter::IRC::Client::data_->delChannel(i);
										Jupiter::IRC::Client::Channel *channel = Jupiter::IRC::Client::data_->channels.get(Jupiter::IRC::Client::data_->addChannel(chan.c_str()));
										channel->data_->isAddingNames = true;
										if (channel->getType() < 0)
										{
											if (Jupiter::IRC::Client::data_->autoPartMessage.size() != 0) Jupiter::IRC::Client::partChannel(chan.c_str(), Jupiter::IRC::Client::data_->autoPartMessage.c_str());
											else Jupiter::IRC::Client::partChannel(chan.c_str());
										}
									}
									else if (i >= 0) Jupiter::IRC::Client::data_->channels.get(i)->addUser(Jupiter::IRC::Client::data_->findUserOrAdd(nick));
									this->OnJoin(chan, nick);
									for (i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnJoin(this, chan, nick);
								}
								else if (streqli(w2.c_str(), "PART"))
								{
									auto nick = getSender(buff);
									if (nick.size() != 0)
									{
										auto chan = buff.getWord(2, WHITESPACE);
										if (chan.size() != 0)
										{
											int i = Jupiter::IRC::Client::getChannelIndex(chan.c_str());
											if (i >= 0)
											{
												int userIndex = Jupiter::IRC::Client::getUserIndex(nick.c_str());
												if (userIndex >= 0)
												{
													Jupiter::IRC::Client::User *user = Jupiter::IRC::Client::data_->users.get(userIndex);
													Jupiter::IRC::Client::data_->channels.get(i)->delUser(nick.c_str());
													int pos = findSymbol(buff.c_str(), ':', 1);
													Jupiter::CStringS reason = buff.c_str() + pos + 1;
													this->OnPart(chan, nick, reason);
													for (i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnPart(this, chan, nick, reason);
													if (nick.equalsi(Jupiter::IRC::Client::data_->nickname)) Jupiter::IRC::Client::data_->delChannel(chan.c_str());
													if (user->getChannelCount() == 0) Jupiter::IRC::Client::data_->users.remove(userIndex);
												}
											}
										}
									}
								}
								else if (streqli(w2.c_str(), "KICK"))
								{
									auto chan = buff.getWord(2, WHITESPACE);
									if (chan.size() != 0)
									{
										Jupiter::CStringS kicker = getSender(buff);
										if (kicker.size() != 0)
										{
											auto kicked = buff.getWord(3, WHITESPACE);
											if (kicked.size() != 0)
											{
												int i = Jupiter::IRC::Client::getChannelIndex(chan.c_str());
												if (i >= 0)
												{
													int userIndex = Jupiter::IRC::Client::getUserIndex(kicked.c_str());
													if (userIndex >= 0)
													{
														Jupiter::IRC::Client::User *user = Jupiter::IRC::Client::data_->users.get(userIndex);
														Jupiter::IRC::Client::data_->channels.get(i)->delUser(kicked.c_str());
														int pos = findSymbol(buff.c_str(), ':', 1);
														Jupiter::CStringS reason;
														if (pos >= 0) reason += buff.c_str() + pos + 1;
														this->OnKick(chan, kicker, kicked, reason);
														for (i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnKick(this, chan, kicker, kicked, reason);
														if (kicked.equalsi(Jupiter::IRC::Client::data_->nickname))
														{
															Jupiter::IRC::Client::data_->delChannel(chan.c_str());
															if (Jupiter::IRC::Client::data_->joinOnKick) Jupiter::IRC::Client::joinChannel(chan.c_str());
														}
														if (user->getChannelCount() == 0) Jupiter::IRC::Client::data_->users.remove(userIndex);
													}
												}
											}
										}
									}
								}
								else if (streqli(w2.c_str(), "QUIT"))
								{
									Jupiter::CStringS nick = getSender(buff);
									Jupiter::CStringS message = buff.substring(findSymbol(buff.c_str(), ':', 1) + 1, buff.size());
									int userIndex = Jupiter::IRC::Client::getUserIndex(nick.c_str());
									if (userIndex >= 0)
									{
										Jupiter::IRC::Client::User *user = Jupiter::IRC::Client::data_->users.get(userIndex);
										unsigned int i;
										for (i = 0; i < Jupiter::IRC::Client::data_->channels.size(); i++) Jupiter::IRC::Client::data_->channels.get(i)->delUser(nick.c_str());
										this->OnQuit(nick, message);
										for (i = 0; i < Jupiter::plugins->size(); i++) Jupiter::plugins->get(i)->OnQuit(this, nick, message);
										if (user->getChannelCount() == 0) Jupiter::IRC::Client::data_->users.remove(userIndex);
									}
								}
								else if (streqli(w2.c_str(), "INVITE"))
								{
									Jupiter::CStringS inviter = getSender(buff);
									auto invited = buff.getWord(2, WHITESPACE);
									Jupiter::CStringS chan = buff.c_str() + findSymbol(buff.c_str(), ':', 1) + 1;
									this->OnInvite(chan, inviter, invited);
									for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnInvite(this, chan, inviter, invited);
								}
								else if (streqli(w2.c_str(), "MODE"))
								{
									auto chan = buff.getWord(2, WHITESPACE);
									if (chan.size() != 0)
									{
										if (containsSymbol(Jupiter::IRC::Client::data_->chanTypes.c_str(), chan[0]))
										{
											auto nick = getSender(buff);
											if (nick.size() != 0)
											{
												const char *modestring = buff.c_str() + findSymbol(buff.c_str(), ' ', 2) + 1;
												if (wordCount(modestring) > 1)
												{
													char *modes = getWord(modestring, 0);
													if (modes != nullptr)
													{
														const char *params = modestring + findSymbol(modestring, ' ', 0) + 1;
														char *tword;
														unsigned char g = 0;
														char symb = 0;
														for (uint8_t z = 0; modes[z] != 0; z++)
														{
															if (modes[z] == '+' || modes[z] == '-') symb = modes[z];
															else if (findSymbol(Jupiter::IRC::Client::data_->prefixModes.c_str(), modes[z], 0) >= 0)
															{
																tword = getWord(params, g);
																if (tword != nullptr)
																{
																	Jupiter::IRC::Client::Channel *channel;
																	for (unsigned int channelIndex = 0; channelIndex < Jupiter::IRC::Client::data_->channels.size(); channelIndex++)
																	{
																		channel = Jupiter::IRC::Client::data_->channels.get(channelIndex);
																		if (channel->getName().equalsi(chan.c_str()))
																		{
																			if (symb == '+') channel->addUserPrefix(tword, Jupiter::IRC::Client::data_->prefixes[findSymbol(Jupiter::IRC::Client::data_->prefixModes.c_str(), modes[z], 0)]);
																			else channel->delUserPrefix(tword, Jupiter::IRC::Client::data_->prefixes[findSymbol(Jupiter::IRC::Client::data_->prefixModes.c_str(), modes[z], 0)]);
																			break;
																		}
																	}
																	free(tword);
																}
																g++;
															}
															else
															{
																if (containsSymbol(Jupiter::IRC::Client::data_->modeA.c_str(), modes[z])) g++;
																else if (containsSymbol(Jupiter::IRC::Client::data_->modeB.c_str(), modes[z])) g++;
																else if (containsSymbol(Jupiter::IRC::Client::data_->modeC.c_str(), modes[z]) && symb == '+') g++;
															}
														}
														free(modes);
													}
												}
												Jupiter::CStringS modeString = modestring;
												this->OnMode(chan, nick, modeString);
												for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnMode(this, chan, nick, modeString);
											}
										}
									}
								}
								// else if ACCOUNT
								// else if CHGHOST
								else if (numeric == IRC_RPL_NAMREPLY) // Some names.
								{
									Jupiter::CStringS chan = buff.getWord(4, " ");
									Jupiter::CStringS names = buff.c_str() + findSymbol(buff.c_str(), ':', 1) + 1;
									int i = Jupiter::IRC::Client::getChannelIndex(chan.c_str());
									if (i >= 0)
									{
										if (Jupiter::IRC::Client::getChannel(i)->data_->isAddingNames == false)
										{
											Jupiter::IRC::Client::data_->delChannel(i);
											Jupiter::IRC::Client::getChannel(Jupiter::IRC::Client::data_->addChannel(chan.c_str()))->data_->isAddingNames = true;
										}
										Jupiter::IRC::Client::data_->addNamesToChannel(i, names);
									}
								}
								else if (numeric == IRC_RPL_ENDOFNAMES) // We're done here.
								{
									Jupiter::CStringS chan = buff.getWord(3, " ");
									int i = Jupiter::IRC::Client::getChannelIndex(chan.c_str());
									if (i >= 0) Jupiter::IRC::Client::data_->channels.get(i)->data_->isAddingNames = false;
								}
							}
							break;
						}
					}
					else
					{
						if (w1.equals("PING"))
						{
							char *tempstr;
							tempstr = new char[w2.size() + 8];
							sprintf(tempstr, "PONG %s" ENDL, w2.c_str());
							Jupiter::IRC::Client::data_->sock->send(tempstr);
							delete[] tempstr;
						}
						else if (w1.equals("NICK"))
						{
							if (w2.size() != 0) Jupiter::IRC::Client::data_->nickname = w2;
						}
						else if (w1.equals("ERROR"))
						{
							Jupiter::CStringS reason = buff.c_str() + strcspn(buff.c_str(), ":") + 1;
							this->OnError(reason);
							for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnError(this, reason);
							Jupiter::IRC::Client::disconnect();
						}
						else if (w1.equals("AUTHENTICATE"))
						{
							if (Jupiter::IRC::Client::data_->saslPass.size() != 0)
							{
								size_t authStringLen = Jupiter::IRC::Client::data_->nickname.size() + Jupiter::IRC::Client::data_->saslAccount.size() + Jupiter::IRC::Client::data_->saslPass.size() + 2;
								char *authString = new char[authStringLen + 1];
								int offset = sprintf(authString, "%s", Jupiter::IRC::Client::data_->nickname.c_str()) + 1;
								offset += sprintf(authString + offset, "%s", Jupiter::IRC::Client::data_->saslAccount.c_str()) + 1;
								offset += sprintf(authString + offset, "%s", Jupiter::IRC::Client::data_->saslPass.c_str());

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
						for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnNumeric(this, numeric, buff);
					}
				}
				this->OnRaw(buff);
				for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnRaw(this, buff);
			}
		}
		return 0;
	}
	int lastError = Jupiter::IRC::Client::data_->sock->getLastError();
	if (lastError == 10035) return 0;
	printf(ENDL ENDL "recv Value: %d -- Conneciton Error: %d -- Connection Status: %d" ENDL ENDL ENDL, recvVal, lastError, Jupiter::IRC::Client::data_->connectionStatus);
	if (Jupiter::IRC::Client::data_->connectionStatus) Jupiter::IRC::Client::disconnect();
	return lastError;
}

bool Jupiter::IRC::Client::connect()
{
	const char *clientAddress = Jupiter::IRC::Client::readConfigValue("ClientAddress").c_str();
	if (*clientAddress == 0) clientAddress = nullptr;
	bool r = Jupiter::IRC::Client::data_->sock->connectToHost(Jupiter::IRC::Client::data_->serverHostname.c_str(), Jupiter::IRC::Client::data_->serverPort, clientAddress, (unsigned short) Jupiter::IRC::Client::readConfigLong("ClientPort"));
	if (r)
	{
		Jupiter::IRC::Client::data_->sock->setBlocking(false);
		if (Jupiter::IRC::Client::data_->ssl == false)
		{
			Jupiter::IRC::Client::data_->sock->send("STARTTLS" ENDL);
			Jupiter::IRC::Client::data_->connectionStatus = 1;
		}
		else Jupiter::IRC::Client::data_->startCAP();
	}
	return r;
}

void Jupiter::IRC::Client::disconnect(bool stayDead)
{
	Jupiter::IRC::Client::data_->channels.emptyAndDelete();
	Jupiter::IRC::Client::data_->connectionStatus = 0;
	Jupiter::IRC::Client::data_->sock->closeSocket();
	Jupiter::IRC::Client::data_->reconnectTime = time(0) + Jupiter::IRC::Client::data_->reconnectDelay;
	Jupiter::IRC::Client::data_->dead = stayDead;
	this->OnDisconnect();
	for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnDisconnect(this);
}

void Jupiter::IRC::Client::disconnect(const char *message, bool stayDead)
{
	char *msg = new char[strlen(message) + 9];
	sprintf(msg, "QUIT :%s" ENDL, message);
	Jupiter::IRC::Client::data_->sock->send(msg);
	delete[] msg;
	Jupiter::IRC::Client::disconnect(stayDead);
}

void Jupiter::IRC::Client::reconnect()
{
	if (Jupiter::IRC::Client::data_->connectionStatus) Jupiter::IRC::Client::disconnect();
	Jupiter::IRC::Client::data_->reconnectAttempts++;
	bool successConnect = Jupiter::IRC::Client::connect();
	this->OnReconnectAttempt(successConnect);
	for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnReconnectAttempt(this, successConnect);
}

int Jupiter::IRC::Client::think()
{
	int r = Jupiter::IRC::Client::primaryHandler();
	for (int i = Jupiter::plugins->size() - 1; i >= 0; i--) Jupiter::plugins->get(i)->OnThink(this);
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

Jupiter::CStringS Jupiter::IRC::Client::readConfigValue(const char *key, const char *defaultValue) const
{
	const char *val = Jupiter::IRC::Client::Config->get(Jupiter::IRC::Client::data_->configSectionName.c_str(), key);
	if (val == nullptr) val = Jupiter::IRC::Client::Config->get("Default", key, defaultValue);
	return val;
}

bool Jupiter::IRC::Client::readConfigBool(const char *key, bool defaultValue) const
{
	if (Jupiter::IRC::Client::Config->get(Jupiter::IRC::Client::data_->configSectionName.c_str(), key)) return Jupiter::IRC::Client::Config->getBool(Jupiter::IRC::Client::data_->configSectionName.c_str(), key);
	return Jupiter::IRC::Client::Config->getBool("Default", key, defaultValue);
}

int Jupiter::IRC::Client::readConfigInt(const char *key, int defaultValue) const
{
	if (Jupiter::IRC::Client::Config->get(Jupiter::IRC::Client::data_->configSectionName.c_str(), key)) return Jupiter::IRC::Client::Config->getInt(Jupiter::IRC::Client::data_->configSectionName.c_str(), key);
	return Jupiter::IRC::Client::Config->getInt("Default", key, defaultValue);
}

long Jupiter::IRC::Client::readConfigLong(const char *key, long defaultValue) const
{
	if (Jupiter::IRC::Client::Config->get(Jupiter::IRC::Client::data_->configSectionName.c_str(), key)) return Jupiter::IRC::Client::Config->getLong(Jupiter::IRC::Client::data_->configSectionName.c_str(), key);
	return Jupiter::IRC::Client::Config->getLong("Default", key, defaultValue);
}

double Jupiter::IRC::Client::readConfigDouble(const char *key, double defaultValue) const
{
	if (Jupiter::IRC::Client::Config->get(Jupiter::IRC::Client::data_->configSectionName.c_str(), key)) return Jupiter::IRC::Client::Config->getDouble(Jupiter::IRC::Client::data_->configSectionName.c_str(), key);
	return Jupiter::IRC::Client::Config->getDouble("Default", key, defaultValue);
}

void Jupiter::IRC::Client::writeToLogs(const char *message)
{
	if (Jupiter::IRC::Client::data_->logFile != nullptr)
	{
		fputs(message, Jupiter::IRC::Client::data_->logFile);
		fputs(ENDL, Jupiter::IRC::Client::data_->logFile);
		fflush(Jupiter::IRC::Client::data_->logFile);
	}
}

/**
* @brief IRC Client Data Implementation
*/

void Jupiter::IRC::Client::Data::delChannel(const char *chan)
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

Jupiter::IRC::Client::User *Jupiter::IRC::Client::Data::findUser(const char *nick) const
{
	Jupiter::IRC::Client::User *r;
	for (unsigned int i = 0; i < Jupiter::IRC::Client::Data::users.size(); i++)
	{
		r = Jupiter::IRC::Client::Data::users.get(i);
		if (r->getNickname().equalsi(nick)) return r;
	}
	return nullptr;
}

Jupiter::IRC::Client::User *Jupiter::IRC::Client::Data::findUserOrAdd(Jupiter::CStringS &name)
{
	unsigned int wc = name.wordCount("!@");
	Jupiter::CStringS nick = (wc == 1) ? name : name.getWord(0, "!@");
	Jupiter::IRC::Client::User *r = Jupiter::IRC::Client::Data::findUser(nick.c_str());
	if (r == nullptr)
	{
		r = new Jupiter::IRC::Client::User();
		switch (wc)
		{
		case 3:
			r->data_->hostname = name.getWord(2, "!@");
		case 2: // This shouldn't EVER happen.
			r->data_->username = name.getWord(1, "!@");
		case 1: // No user/host is in the string.
			r->data_->nickname = nick;
			break;
		default:
			fprintf(stderr, "ERROR: Failed to parse name mask: %s" ENDL, name.c_str());
			break;
		}
		Jupiter::IRC::Client::Data::users.add(r);
	}
	return r;
}

void Jupiter::IRC::Client::Data::addNamesToChannel(unsigned int index, Jupiter::CStringS &names)
{
	Jupiter::CStringS t;
	size_t offset;
	Jupiter::IRC::Client::Channel *channel = Jupiter::IRC::Client::Data::channels.get(index);
	int nameLen = names.wordCount(" ");
	for (short i = 0; i < nameLen; i++)
	{
		t = names.getWord(i, " ");
		if (t.size() != 0)
		{
			offset = strspn(t.c_str(), Jupiter::IRC::Client::Data::prefixes.c_str());
			t.shiftRight(offset);
			Jupiter::IRC::Client::User *user = Jupiter::IRC::Client::Data::findUserOrAdd(t);
			t.shiftLeft(offset);
			unsigned int userIndex = channel->addUser(user);
			if (offset > 0)
			{
				while (offset > 0)
				{
					offset--;
					channel->addUserPrefix(userIndex, t[offset]);
				}
			}
		}
	}
}

unsigned int Jupiter::IRC::Client::Data::addChannel(const char *chan)
{
	Jupiter::IRC::Client::Data::channels.add(new Channel(chan, Jupiter::IRC::Client::Data::iFace));
	return Jupiter::IRC::Client::Data::channels.size() - 1;
}

bool Jupiter::IRC::Client::Data::startCAP()
{
	Jupiter::IRC::Client::Data::connectionStatus = 2;
	return Jupiter::IRC::Client::Data::sock->send("CAP LS" ENDL, 8) > 0;
}

bool Jupiter::IRC::Client::Data::registerClient()
{
	bool r = true;
	const char *localHostname = Jupiter::Socket::getLocalHostname();
	Jupiter::CStringL messageToSend;
	messageToSend.format("USER %s %s %s :%s" ENDL, Jupiter::IRC::Client::Data::nickname.c_str(), localHostname, Jupiter::IRC::Client::Data::serverHostname.c_str(), Jupiter::IRC::Client::Data::realName.c_str());
	if (Jupiter::IRC::Client::Data::sock->send(messageToSend.c_str(), messageToSend.size()) <= 0) r = false;
	messageToSend.format("NICK %s" ENDL, Jupiter::IRC::Client::Data::nickname.c_str());
	if (Jupiter::IRC::Client::Data::sock->send(messageToSend.c_str(), messageToSend.size()) <= 0) r = false;
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

const Jupiter::StringType &Jupiter::IRC::Client::User::getNickname() const
{
	return Jupiter::IRC::Client::User::data_->nickname;
}

const Jupiter::StringType &Jupiter::IRC::Client::User::getUsername() const
{
	return Jupiter::IRC::Client::User::data_->username;
}

const Jupiter::StringType &Jupiter::IRC::Client::User::getHostname() const
{
	return Jupiter::IRC::Client::User::data_->hostname;
}

unsigned int Jupiter::IRC::Client::User::getChannelCount() const
{
	return Jupiter::IRC::Client::User::data_->channelCount;
}

/**
* Channel Implementation
*/

Jupiter::IRC::Client::Channel::Channel(const char *channelName, Jupiter::IRC::Client *iFace)
{
	Jupiter::IRC::Client::Channel::data_ = new Jupiter::IRC::Client::Channel::Data();
	unsigned int i = 0;
	const char *ptr = nullptr;
	const char *section = iFace->getConfigSection().c_str();
	char key[64];
	int offset = sprintf(key, "Channel.");
	Jupiter::CStringL channelKeyStr;

	// TODO: Make this more efficient -- consider moving responsibility elsewhere.
	while (1)
	{
		i++;
		sprintf(key + offset, "%u", i);
		ptr = Jupiter::IRC::Client::Config->get(section, key);
		if (ptr == nullptr)
		{
			if (section != iFace->getConfigSection().c_str()) break;
			i = 0;
			section = "Default";
			continue;
		}
		channelKeyStr = ptr;
		if (channelKeyStr.getWord(0, WHITESPACE).equalsi(channelName))
		{
			sprintf(key + offset, "%u.Type", i);
			ptr = Jupiter::IRC::Client::Config->get(section, key);
			if (ptr != nullptr)
			{
				Jupiter::IRC::Client::Channel::data_->type = atoi(ptr);
				break;
			}
		}
	}
	if (ptr == nullptr)
	{
		sprintf(key + offset, "%s.Type", channelName);
		ptr = iFace->readConfigValue(key).c_str();
		if (*ptr != 0) Jupiter::IRC::Client::Channel::data_->type = atoi(ptr);
		else Jupiter::IRC::Client::Channel::data_->type = iFace->getDefaultChanType();
	}
	Jupiter::IRC::Client::Channel::data_->channel = channelName;
	Jupiter::IRC::Client::Channel::data_->serverPrefixSetPtr = iFace->getPrefixes().c_str();
}

Jupiter::IRC::Client::Channel::~Channel()
{
	while (Jupiter::IRC::Client::Channel::data_->users.size() != 0) delete Jupiter::IRC::Client::Channel::data_->users.remove(0);
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

void Jupiter::IRC::Client::Channel::delUser(const char *nickname)
{
	int index = getUserIndex(nickname);
	if (index >= 0) Jupiter::IRC::Client::Channel::delUser(index);
}

void Jupiter::IRC::Client::Channel::delUser(unsigned int index)
{
	if (index < Jupiter::IRC::Client::Channel::data_->users.size())
	{
		Jupiter::IRC::Client::Channel::User *user = Jupiter::IRC::Client::Channel::data_->users.remove(index);
		user->data_->user->data_->channelCount--;
		delete user;
	}
}

void Jupiter::IRC::Client::Channel::addUserPrefix(unsigned int index, char prefix)
{
	if (index < Jupiter::IRC::Client::Channel::data_->users.size())
	{
		if (Jupiter::IRC::Client::Channel::data_->users.get(index)->data_->prefixes.contains(prefix) == false)
		{
			Jupiter::IRC::Client::Channel::data_->users.get(index)->data_->prefixes += prefix;
		}
	}
}

void Jupiter::IRC::Client::Channel::addUserPrefix(const char *user, char prefix)
{
	int i = Jupiter::IRC::Client::Channel::getUserIndex(user);
	if (i >= 0) Jupiter::IRC::Client::Channel::addUserPrefix(i, prefix);
}

void Jupiter::IRC::Client::Channel::delUserPrefix(unsigned int index, char prefix)
{
	if (index < Jupiter::IRC::Client::Channel::data_->users.size()) Jupiter::IRC::Client::Channel::data_->users.get(index)->data_->prefixes.remove(prefix);
}

void Jupiter::IRC::Client::Channel::delUserPrefix(const char *user, char prefix)
{
	int i = Jupiter::IRC::Client::Channel::getUserIndex(user);
	if (i >= 0) Jupiter::IRC::Client::Channel::delUserPrefix(i, prefix);
}

const Jupiter::StringType &Jupiter::IRC::Client::Channel::getName() const
{
	return Jupiter::IRC::Client::Channel::data_->channel;
}

Jupiter::IRC::Client::Channel::User *Jupiter::IRC::Client::Channel::getUser(unsigned int index) const
{
	Jupiter::ArrayList<Jupiter::IRC::Client::Channel::User> &users = Jupiter::IRC::Client::Channel::data_->users;
	if (index < users.size()) return users.get(index);
	return nullptr;
}

Jupiter::IRC::Client::Channel::User *Jupiter::IRC::Client::Channel::getUser(const char *nickname) const
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

int Jupiter::IRC::Client::Channel::getUserIndex(const char *user) const
{
	for (unsigned int i = 0; i < Jupiter::IRC::Client::Channel::data_->users.size(); i++) if (Jupiter::IRC::Client::Channel::data_->users[i] && Jupiter::IRC::Client::Channel::data_->users[i]->data_->user->getNickname().equalsi(user)) return i;
	return -1;
}

int Jupiter::IRC::Client::Channel::getUserIndexByPartName(const char *user) const
{
	for (unsigned int i = 0; i < Jupiter::IRC::Client::Channel::data_->users.size(); i++) if (Jupiter::IRC::Client::Channel::data_->users[i] && stristr(Jupiter::IRC::Client::Channel::data_->users[i]->data_->user->getNickname().c_str(), user)) return i;
	return -1;
}

char Jupiter::IRC::Client::Channel::getUserPrefix(unsigned int index) const
{
	if (index < Jupiter::IRC::Client::Channel::data_->users.size())
		for (unsigned char i = 0; Jupiter::IRC::Client::Channel::data_->serverPrefixSetPtr[i]; i++)
			if (containsSymbol(Jupiter::IRC::Client::Channel::data_->users.get(index)->data_->prefixes.c_str(), Jupiter::IRC::Client::Channel::data_->serverPrefixSetPtr[i])) return Jupiter::IRC::Client::Channel::data_->serverPrefixSetPtr[i];
	return 0;
}

char Jupiter::IRC::Client::Channel::getUserPrefix(const char *user) const
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

const Jupiter::StringType &Jupiter::IRC::Client::Channel::User::getPrefixes() const
{
	return Jupiter::IRC::Client::Channel::User::data_->prefixes;
}

const Jupiter::StringType &Jupiter::IRC::Client::Channel::User::getNickname() const
{
	return Jupiter::IRC::Client::Channel::User::data_->user->getNickname();
}

const Jupiter::StringType &Jupiter::IRC::Client::Channel::User::getUsername() const
{
	return Jupiter::IRC::Client::Channel::User::data_->user->getUsername();
}

const Jupiter::StringType &Jupiter::IRC::Client::Channel::User::getHostname() const
{
	return Jupiter::IRC::Client::Channel::User::data_->user->getHostname();
}

unsigned int Jupiter::IRC::Client::Channel::User::getChannelCount() const
{
	return Jupiter::IRC::Client::Channel::User::data_->user->getChannelCount();
}