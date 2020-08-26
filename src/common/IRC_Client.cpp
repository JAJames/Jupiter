/**
 * Copyright (C) 2013-2017 Jessica James.
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
#include "TCPSocket.h"
#include "String.hpp"
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

Jupiter::IRC::Client::Client(Jupiter::Config *in_primary_section, Jupiter::Config *in_secondary_section)
{
	m_primary_section = in_primary_section;
	m_secondary_section = in_secondary_section;

	if (m_primary_section != nullptr)
		m_primary_section_name = m_primary_section->getName();

	m_server_hostname = static_cast<std::string>(Jupiter::IRC::Client::readConfigValue("Hostname"_jrs, "irc.cncirc.net"_jrs));
	
	m_log_file_name = static_cast<std::string>(Jupiter::IRC::Client::readConfigValue("LogFile"_jrs));
	m_nickname = Jupiter::IRC::Client::readConfigValue("Nick"_jrs, "Jupiter"_jrs);

	m_realname = Jupiter::IRC::Client::readConfigValue("RealName"_jrs, "Jupiter IRC Client"_jrs);

	m_sasl_password = Jupiter::IRC::Client::readConfigValue("SASL.Password"_jrs);
	if (m_sasl_password.isEmpty())
		m_sasl_password = Jupiter::IRC::Client::readConfigValue("SASL.Pass"_jrs);
	
	m_sasl_account = Jupiter::IRC::Client::readConfigValue("SASL.Account"_jrs);
	if (m_sasl_account.isEmpty())
		m_sasl_account = m_nickname;

	m_auto_part_message = Jupiter::IRC::Client::readConfigValue("AutoPartMessage"_jrs);

	m_ssl = Jupiter::IRC::Client::readConfigBool("SSL"_jrs);
	m_ssl_certificate = Jupiter::IRC::Client::readConfigValue("Certificate"_jrs);
	m_ssl_key = Jupiter::IRC::Client::readConfigValue("Key"_jrs);
	if (m_ssl_certificate.isEmpty())
	{
		m_ssl_certificate = Jupiter::IRC::Client::readConfigValue("Cert"_jrs);
		if (m_ssl_certificate.isEmpty())
			m_ssl_certificate = m_ssl_key;
	}
	if (m_ssl_key.isEmpty())
		m_ssl_key = m_ssl_certificate;

	m_join_on_kick = Jupiter::IRC::Client::readConfigBool("AutoJoinOnKick"_jrs);
	m_reconnect_delay = Jupiter::IRC::Client::readConfigInt("AutoReconnectDelay"_jrs);
	m_max_reconnect_attempts = Jupiter::IRC::Client::readConfigInt("MaxReconnectAttempts"_jrs);
	m_server_port = (unsigned short)Jupiter::IRC::Client::readConfigInt("Port"_jrs, m_ssl ? 994 : 194);
	m_default_chan_type = Jupiter::IRC::Client::readConfigInt("Channel.Type"_jrs);

	if (Jupiter::IRC::Client::readConfigBool("PrintOutput"_jrs, true))
		m_output = stdout;
	else
		m_output = nullptr;
	if (!m_log_file_name.empty())
		m_log_file = fopen(m_log_file_name.c_str(), "a+b");
	else m_log_file = nullptr;

	if (m_ssl)
	{
		Jupiter::SecureTCPSocket *t = new Jupiter::SecureTCPSocket();
		if (m_ssl_certificate.isNotEmpty())
			t->setCertificate(m_ssl_certificate, m_ssl_key);
		m_socket = t;
	}
	else m_socket = new Jupiter::TCPSocket();

	m_connection_status = 0;
}

Jupiter::IRC::Client::~Client()
{
	m_socket->close();

	if (m_socket != nullptr)
		delete m_socket;

	if (m_log_file != nullptr)
		fclose(m_log_file);
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
	if (m_primary_section != nullptr)
		return m_primary_section_name;

	return Jupiter::ReferenceString::empty;
}

Jupiter::Config *Jupiter::IRC::Client::getPrimaryConfigSection() const
{
	return m_primary_section;
}

Jupiter::Config *Jupiter::IRC::Client::getSecondaryConfigSection() const
{
	return m_secondary_section;
}

void Jupiter::IRC::Client::setPrimaryConfigSection(Jupiter::Config *in_primary_section)
{
	m_primary_section = in_primary_section;

	if (m_primary_section != nullptr)
		m_primary_section_name = m_primary_section->getName();
	else
		m_primary_section_name.erase();
}

void Jupiter::IRC::Client::setSecondaryConfigSection(Jupiter::Config *in_secondary_section)
{
	m_secondary_section = in_secondary_section;
}

const std::string &Jupiter::IRC::Client::getLogFile() const
{
	return m_log_file_name;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getPrefixes() const
{
	return m_prefixes;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getPrefixModes() const
{
	return m_prefix_modes;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getNickname() const
{
	return m_nickname;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getRealname() const
{
	return m_realname;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getServerName() const
{
	return m_server_name;
}

const std::string &Jupiter::IRC::Client::getServerHostname() const
{
	return m_server_hostname;
}

unsigned short Jupiter::IRC::Client::getServerPort() const
{
	return m_server_port;
}

time_t Jupiter::IRC::Client::getReconnectDelay() const
{
	return m_reconnect_delay;
}

time_t Jupiter::IRC::Client::getReconnectTime() const
{
	return m_reconnect_time;
}

int Jupiter::IRC::Client::getReconnectAttempts() const
{
	return m_reconnect_attempts;
}

int Jupiter::IRC::Client::getMaxReconnectAttempts() const
{
	return m_max_reconnect_attempts;
}

int Jupiter::IRC::Client::getDefaultChanType() const
{
	return m_default_chan_type;
}

FILE *Jupiter::IRC::Client::getPrintOutput() const
{
	return m_output;
}

void Jupiter::IRC::Client::setPrintOutput(FILE *f)
{
	m_output = f;
}

inline Jupiter::ReferenceString getSender(const Jupiter::ReadableString &line)
{
	return Jupiter::ReferenceString::getWord(line, 0, ":! ");
}

int Jupiter::IRC::Client::getAccessLevel(const Channel &in_channel, const Jupiter::ReadableString &in_nickname) const
{
	char prefix = in_channel.getUserPrefix(in_nickname);

	if (prefix != 0)
		return static_cast<int>(m_prefixes.size() - m_prefixes.find(prefix));

	return 0;
}

int Jupiter::IRC::Client::getAccessLevel(const Jupiter::ReadableString &in_channel, const Jupiter::ReadableString &in_nickname) const
{
	auto channel = m_channels.find(in_channel);

	if (channel != m_channels.end())
		return this->getAccessLevel(channel->second, in_nickname);

	return 0;
}

void Jupiter::IRC::Client::send(const Jupiter::ReadableString &rawMessage)
{
	Jupiter::StringS out = rawMessage;
	out += ENDL;

	m_socket->send(out);
}

const Jupiter::IRC::Client::UserTableType &Jupiter::IRC::Client::getUsers() const
{
	return m_users;
}

size_t Jupiter::IRC::Client::getUserCount() const
{
	return m_users.size();
}

std::shared_ptr<Jupiter::IRC::Client::User> Jupiter::IRC::Client::getUser(const Jupiter::ReadableString &in_nickname) const
{
	auto user = m_users.find(in_nickname);
	if (user != m_users.end()) {
		return user->second;
	}

	return nullptr;
}

const Jupiter::IRC::Client::ChannelTableType &Jupiter::IRC::Client::getChannels() const
{
	return m_channels;
}

size_t Jupiter::IRC::Client::getChannelCount() const
{
	return m_channels.size();
}

Jupiter::IRC::Client::Channel *Jupiter::IRC::Client::getChannel(const Jupiter::ReadableString &in_channel) const
{
	auto channel = m_channels.find(in_channel);
	if (channel != m_channels.end()) {
		return const_cast<Channel*>(&channel->second);
	}

	return nullptr;
}

bool Jupiter::IRC::Client::isAutoReconnect() const
{
	return m_max_reconnect_attempts != 0;
}

void Jupiter::IRC::Client::setAutoReconnect(int val)
{
	m_max_reconnect_attempts = val;
}

void Jupiter::IRC::Client::joinChannel(const Jupiter::ReadableString &in_channel)
{
	m_socket->send(Jupiter::StringS::Format("JOIN %.*s" ENDL, in_channel.size(), in_channel.ptr()));
}

void Jupiter::IRC::Client::joinChannel(const Jupiter::ReadableString &in_channel, const Jupiter::ReadableString &in_password)
{
	m_socket->send(Jupiter::StringS::Format("JOIN %.*s %.*s" ENDL, in_channel.size(), in_channel.ptr(), in_password.size(), in_password.ptr()));
}

void Jupiter::IRC::Client::partChannel(const Jupiter::ReadableString &in_channel)
{
	m_socket->send(Jupiter::StringS::Format("PART %.*s" ENDL, in_channel.size(), in_channel.ptr()));

	m_channels[in_channel].setType(-2);
}

void Jupiter::IRC::Client::partChannel(const Jupiter::ReadableString &in_channel, const Jupiter::ReadableString &in_message)
{
	m_socket->send(Jupiter::StringS::Format("PART %.*s :%.*s" ENDL, in_channel.size(), in_channel.ptr(), in_message.size(), in_message.ptr()));
	
	m_channels[in_channel].setType(-2);
}

void Jupiter::IRC::Client::sendMessage(const Jupiter::ReadableString &dest, const Jupiter::ReadableString &message)
{
	m_socket->send(Jupiter::StringS::Format("PRIVMSG %.*s :%.*s" ENDL, dest.size(), dest.ptr(), message.size(), message.ptr()));
}

void Jupiter::IRC::Client::sendNotice(const Jupiter::ReadableString &dest, const Jupiter::ReadableString &message)
{
	m_socket->send(Jupiter::StringS::Format("NOTICE %.*s :%.*s" ENDL, dest.size(), dest.ptr(), message.size(), message.ptr()));
}

size_t Jupiter::IRC::Client::messageChannels(int type, const Jupiter::ReadableString &message)
{
	for (auto& channel : m_channels) {
		if (channel.second.getType() == type) {
			sendMessage(channel.second.getName(), message);
		}
	}

	return m_channels.size();
}

size_t Jupiter::IRC::Client::messageChannels(const Jupiter::ReadableString &message)
{
	for (auto& channel : m_channels) {
		sendMessage(channel.second.getName(), message);
	}

	return m_channels.size();
}

int Jupiter::IRC::Client::process_line(const Jupiter::ReadableString &line)
{
	if (line.isNotEmpty())
	{
		Jupiter::IRC::Client::writeToLogs(line);
		if (m_output != nullptr)
			line.println(m_output);

		Jupiter::ReferenceString w1 = Jupiter::ReferenceString::getWord(line, 0, WHITESPACE);
		if (w1.isNotEmpty())
		{
			Jupiter::ReferenceString w2 = Jupiter::ReferenceString::getWord(line, 1, WHITESPACE);
			int numeric = w2.asInt(10);
			if (w1[0] == ':') //Messages
			{
				if (w2.isNotEmpty())
				{
					switch (numeric) // Numerics that don't rely on a specific connectionStatus.
					{
					case Reply::BOUNCE: // 010
					{
						Jupiter::ReferenceString portToken = Jupiter::ReferenceString::getWord(line, 4, " ");
						unsigned short port;
						if (portToken[0] == '+') // This is most likely not used anywhere.
						{
							port = (unsigned short)portToken.asUnsignedInt(10);
							if (m_ssl == false)
							{
								m_ssl = true;
								delete m_socket;
								m_socket = new Jupiter::SecureTCPSocket();
							}
						}
						else
						{
							port = (unsigned short)portToken.asUnsignedInt(10);
							if (m_ssl == true)
							{
								m_ssl = false;
								delete m_socket;
								m_socket = new Jupiter::TCPSocket();
							}
						}
						if (port != 0) // Don't default -- could be non-compliant input.
						{
							m_server_hostname = static_cast<std::string>(Jupiter::ReferenceString::getWord(line, 3, WHITESPACE));
							m_server_port = port;
							puts("Reconnecting due to old bounce.");
							this->reconnect();
						}
						else puts("Error: Failed to parse bounce token.");
					}
					break;
					} // numeric switch
					switch (m_connection_status)
					{
					case 1: // Socket established -- attempting STARTTLS
						switch (numeric)
						{
						case Reply::BOUNCEOLD: // 005
							if (line.matchi("*:Try server *, port *"))
							{
								Jupiter::ReferenceString portToken = Jupiter::ReferenceString::getWord(line, 6, " ");
								unsigned short bouncePort = (unsigned short)Jupiter::ReferenceString::getWord(line, 6, " ").asInt(10);

								if (portToken[0] == '+') // This is almost certainly not used anywhere.
								{
									bouncePort = (unsigned short)portToken.asInt(10);
									if (m_ssl == false)
									{
										m_ssl = true;
										delete m_socket;
										m_socket = new Jupiter::SecureTCPSocket();
									}
								}
								else
								{
									bouncePort = (unsigned short)portToken.asInt(10);
									if (m_ssl == true)
									{
										m_ssl = false;
										delete m_socket;
										m_socket = new Jupiter::TCPSocket();
									}
								}
								if (bouncePort != 0)
								{
									auto server_hostname = Jupiter::ReferenceString::getWord(line, 4, " ");
									server_hostname.truncate(1); // trailing comma
									m_server_hostname = static_cast<std::string>(server_hostname);
									m_server_port = bouncePort;
									puts("Reconnecting due to old bounce.");
									this->reconnect();
								}
								else puts("Error: Failed to parse old bounce token.");
							}
							break;

						case Error::UNKNOWNCOMMAND: // 421
						{
							Jupiter::ReferenceString command = Jupiter::ReferenceString::getWord(line, 2, " ");
							if (command.equalsi("STARTTLS")) // Server doesn't support STARTTLS
								Client::startCAP();
						}
						break;

						case Reply::STARTTLS: // 670
						{
							Jupiter::SecureTCPSocket *t = new Jupiter::SecureTCPSocket(std::move(*m_socket));
							delete m_socket;
							m_socket = t;
							m_ssl = true;
							// toggle blocking to prevent error
							if (m_ssl_certificate.isNotEmpty())
								t->setCertificate(m_ssl_certificate, m_ssl_key);

							bool goodSSL;
							if (t->getBlockingMode() == false)
							{
								t->setBlocking(true);
								goodSSL = t->initSSL();
								t->setBlocking(false);
							}
							else goodSSL = t->initSSL();

							if (goodSSL)
								Client::startCAP();
							else
							{
								// Something went wrong. Kill the socket.
								t->close();
							}
						}
						break;

						case Error::STARTTLS: // 691
							Client::startCAP();
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
								Jupiter::ReferenceString w4 = Jupiter::ReferenceString::getWord(line, 3, WHITESPACE);
								if (w4.equals("LS"))
								{
									Jupiter::ReferenceString listParams = Jupiter::ReferenceString::gotoWord(line, 4, WHITESPACE);
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
											if (m_sasl_password.isNotEmpty())
											{
												req += "sasl "_jrs;
												sasl = true;
											}
										}
										// else; // We don't know what this is!
									}
									if (req.size() > 9)
									{
										req -= 1; // Trim off the extra space byte.
										req += ENDL;
										m_socket->send(req);
										if (sasl)
											m_socket->send("AUTHENTICATE PLAIN"_jrs ENDL);
									}
									if (!sasl)
									{
										m_socket->send("CAP END"_jrs ENDL);
										Client::registerClient();
									}
								}
							}
							break;
						case Error::UNKNOWNCOMMAND: // 421
							if (w2.equalsi("CAP")) // Server doesn't support CAP
							{
								Client::registerClient();
							}
							break;
						default:
							break;
						} // numeric switch
						break;

					case 3: // Registration sent, but not verified.
					{
						bool erroneous_nickname = false;
						switch (numeric)
						{
							// We'll take any of these 4, just in-case any of them are missing. In general, this will trigger on 001.
						case Reply::MYINFO: // 004
							m_server_name = Jupiter::ReferenceString::getWord(line, 3, " ");
						case Reply::WELCOME: // 001
						case Reply::YOURHOST: // 002
						case Reply::CREATED: // 003
							m_connection_status = 4;
							break;

							// You have a bad nickname! Try the alt.
							//case Error::NONICKNAMEGIVEN: // 431 -- Not consistently usable due to lack of command field.
						case Error::ERRONEOUSNICKNAME: // 432
							erroneous_nickname = true;
						case Error::NICKNAMEINUSE: // 433
						case Error::NICKCOLLISION: // 436
						case Error::BANNICKCHANGE: // 437 -- Note: This conflicts with another token.
							const Jupiter::ReadableString &altNick = Jupiter::IRC::Client::readConfigValue("AltNick"_jrs);
							const Jupiter::ReadableString &configNick = Jupiter::IRC::Client::readConfigValue("Nick"_jrs, "Jupiter"_jrs);

							if (altNick.isNotEmpty() && m_nickname.equalsi(altNick)) // The alternate nick failed.
							{
								m_nickname = configNick;
								m_nickname += "1";
								
								m_socket->send("NICK "_jrs + m_nickname + ENDL);
							}
							else if (m_nickname.equalsi(configNick)) // The config nick failed
							{
								if (altNick.isEmpty())
								{
									if (erroneous_nickname)
										break; // If this nick is invalid, adding numbers won't help.

									m_nickname += '1';
								}
								else
									m_nickname = altNick;

								m_socket->send("NICK "_jrs + m_nickname + ENDL);
							}
							// Note: Add a series of contains() functions to String_Type.
							else
							{
								if (erroneous_nickname == false) // If this nick is invalid, adding numbers won't help.
								{
									if (m_nickname.size() > configNick.size())
									{
										int n = Jupiter_strtoi_nospace_s(m_nickname.ptr() + configNick.size(), m_nickname.size() - configNick.size(), 10);
										m_nickname.format("%.*s%d", configNick.size(), configNick.ptr(), n + 1);

										m_socket->send("NICK "_jrs + m_nickname + ENDL);
									}
									else
									{
										// Something strange is going on -- did somebody rehash?
										// This can be somewhat edgy -- this will only trigger if someone rehashes AND the new nickname is shorter.
										// However, it won't be fatal even if the new nickname's length is >= the old.
										m_nickname = configNick;
										m_socket->send("NICK "_jrs + m_nickname + ENDL);
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
						case Reply::ISUPPORT: // 005
						{
							size_t pos = line.find("PREFIX=("_jrs);
							if (pos != Jupiter::INVALID_INDEX)
							{
								Jupiter::ReferenceString ref = Jupiter::ReferenceString::substring(line, pos + 8);
								m_prefix_modes = Jupiter::ReferenceString::getWord(ref, 0, ")");
								ref.shiftRight(ref.find(')') + 1);
								m_prefixes = Jupiter::ReferenceString::getWord(ref, 0, " " ENDL);
							}
							pos = line.find("CHANMODES="_jrs);
							if (pos != Jupiter::INVALID_INDEX)
							{
								Jupiter::ReferenceString ref = Jupiter::ReferenceString::substring(line, pos + 10);
								ref = ref.getWord(0, " ");
								size_t pos2 = ref.find(',', 0);
								if (pos != INVALID_INDEX)
								{
									m_modeA = ref.getWord(0, ", ");
									ref.shiftRight(pos + 1);
									pos2 = ref.find(',', 0);
									if (pos != INVALID_INDEX)
									{
										m_modeB = ref.getWord(0, ", ");
										ref.shiftRight(pos + 1);
										pos2 = ref.find(',', 0);
										if (pos != INVALID_INDEX)
										{
											m_modeC = ref.getWord(0, ", ");
											ref.shiftRight(pos + 1);
											m_modeD = ref.getWord(0, ", ");
										}
									}
								}
							}
							pos = line.find("CHANTYPES="_jrs);
							if (pos != Jupiter::INVALID_INDEX)
							{
								Jupiter::ReferenceString ref = Jupiter::ReferenceString::substring(line, pos + 10);
								m_chan_types = ref.getWord(0, " ");
							}
						}
						break;
						case Reply::LUSERCLIENT: // 251
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

							auto join_channels_for_config = [this](Jupiter::Config *config)
							{
								if (config != nullptr) {
									for (auto& section : config->getSections()) {
										if (section.second.get<bool>("AutoJoin"_jrs, false)) {
											this->joinChannel(section.first);
										}
									}
								}
							};

							join_channels_for_config(m_primary_section->getSection("Channels"_jrs));
							join_channels_for_config(m_secondary_section->getSection("Channels"_jrs));

							m_connection_status = 5;
							m_reconnect_attempts = 0;
							this->OnConnect();
							for (i = 0; i < Jupiter::plugins->size(); i++) Jupiter::plugins->get(i)->OnConnect(this);
						}
						break;
						}
						break;

					default: // Post-registration.
						if (w2.equalsi("PRIVMSG"))
						{
							Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(line, 2, WHITESPACE);
							if (chan.isNotEmpty())
							{
								Jupiter::ReferenceString nick = getSender(line);
								if (nick.isNotEmpty())
								{
									Jupiter::ReferenceString premessage = Jupiter::ReferenceString::substring(line, line.find(':', 1) + 1);
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
											m_socket->send(response);
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
							Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(line, 2, WHITESPACE);
							if (chan.isNotEmpty())
							{
								size_t pos = line.find('!', 0);
								auto message = Jupiter::ReferenceString::substring(line, line.find(':', 1) + 1, line.size());
								if (pos < line.find(' '))
								{
									auto nick = Jupiter::ReferenceString::substring(line, 1, pos);
									this->OnNotice(chan, nick, message);
									for (size_t i = 0; i < Jupiter::plugins->size(); i++)
										Jupiter::plugins->get(i)->OnNotice(this, chan, nick, message);
								}
								else
								{
									auto sender = getSender(line);
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
							auto nick = getSender(line);
							Jupiter::ReferenceString newnick = Jupiter::ReferenceString::substring(line, line.find(' ', 1) + 1);
							if (newnick.isNotEmpty() && newnick[0] == ':') newnick.shiftRight(1);
							if (nick.equalsi(m_nickname))
							{
								m_nickname = newnick;
							}
							auto user = Client::findUser(nick);
							if (user != nullptr)
							{
								user->m_nickname = newnick;
								this->OnNick(nick, newnick);
							}
							for (size_t i = 0; i < Jupiter::plugins->size(); i++)
								Jupiter::plugins->get(i)->OnNick(this, nick, newnick);
						}
						else if (w2.equalsi("JOIN"))
						{
							auto nick = getSender(line);
							Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(line, 2, WHITESPACE);
							
							if (chan[0] == ':')
								chan.shiftRight(1);

							auto channel = getChannel(chan);

							if (m_nickname.equalsi(nick))
							{
								// TODO: Optimize by simply wiping channel data, rather than removing and re-adding
								if (channel != nullptr)
									Client::delChannel(channel->getName());

								Client::addChannel(chan);
								channel = getChannel(chan);
								channel->m_adding_names = true;

								if (channel->getType() < 0)
								{
									if (m_auto_part_message.isNotEmpty())
										Jupiter::IRC::Client::partChannel(chan, m_auto_part_message);
									else
										Jupiter::IRC::Client::partChannel(chan);
								}
							}
							else if (channel != nullptr)
								channel->addUser(Client::findUserOrAdd(nick));

							this->OnJoin(chan, nick);

							for (size_t i = 0; i < Jupiter::plugins->size(); i++)
								Jupiter::plugins->get(i)->OnJoin(this, chan, nick);
						}
						else if (w2.equalsi("PART"))
						{
							auto nick = getSender(line);
							if (nick.isNotEmpty())
							{
								Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(line, 2, WHITESPACE);
								if (chan.isNotEmpty())
								{
									Channel *channel = getChannel(chan);
									if (channel != nullptr)
									{
										auto user = getUser(nick);
										if (user != nullptr)
										{
											channel->delUser(nick);
											Jupiter::ReferenceString reason;
											size_t pos = line.find(':', 1);

											if (pos != Jupiter::INVALID_INDEX)
												reason = Jupiter::ReferenceString::substring(line, pos + 1);

											this->OnPart(chan, nick, reason);
											
											for (size_t i = 0; i < Jupiter::plugins->size(); i++)
												Jupiter::plugins->get(i)->OnPart(this, chan, nick, reason);
											
											if (nick.equalsi(m_nickname))
												Client::delChannel(chan);
											
											if (user->getChannelCount() == 0)
												m_users.erase(nick);
										}
									}
								}
							}
						}
						else if (w2.equalsi("KICK"))
						{
							Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(line, 2, WHITESPACE);
							if (chan.isNotEmpty())
							{
								Jupiter::ReferenceString kicker = getSender(line);
								if (kicker.isNotEmpty())
								{
									Jupiter::ReferenceString kicked = Jupiter::ReferenceString::getWord(line, 3, WHITESPACE);
									if (kicked.isNotEmpty())
									{
										Channel *channel = getChannel(chan);
										if (channel != nullptr)
										{
											auto user = getUser(kicked);
											if (user != nullptr)
											{
												channel->delUser(kicked);
												size_t pos = line.find(':', 1);
												Jupiter::ReferenceString reason;

												if (pos != Jupiter::INVALID_INDEX)
													reason = Jupiter::ReferenceString::substring(line, pos + 1);

												this->OnKick(chan, kicker, kicked, reason);

												for (size_t i = 0; i < Jupiter::plugins->size(); i++)
													Jupiter::plugins->get(i)->OnKick(this, chan, kicker, kicked, reason);

												if (kicked.equalsi(m_nickname))
												{
													Client::delChannel(chan);
													if (m_join_on_kick)
														Jupiter::IRC::Client::joinChannel(chan);
												}

												if (user->getChannelCount() == 0)
													m_users.erase(kicked);
											}
										}
									}
								}
							}
						}
						else if (w2.equalsi("QUIT"))
						{
							Jupiter::ReferenceString nick = getSender(line);
							Jupiter::ReferenceString message = Jupiter::ReferenceString::substring(line, line.find(':', 1) + 1);
							auto user = getUser(nick);
							if (user != nullptr)
							{
								for (auto& channel : m_channels) {
									channel.second.delUser(nick);
								}

								this->OnQuit(nick, message);

								for (size_t i = 0; i < Jupiter::plugins->size(); i++)
									Jupiter::plugins->get(i)->OnQuit(this, nick, message);

								m_users.erase(nick);
							}
						}
						else if (w2.equalsi("INVITE"))
						{
							Jupiter::ReferenceString inviter = getSender(line);
							Jupiter::ReferenceString invited = Jupiter::ReferenceString::getWord(line, 2, WHITESPACE);
							Jupiter::ReferenceString chan = Jupiter::ReferenceString::substring(line, line.find(':', 1) + 1);
							this->OnInvite(chan, inviter, invited);
							for (size_t i = 0; i < Jupiter::plugins->size(); i++)
								Jupiter::plugins->get(i)->OnInvite(this, chan, inviter, invited);
						}
						else if (w2.equalsi("MODE"))
						{
							Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(line, 2, WHITESPACE);
							if (chan.isNotEmpty())
							{
								if (m_chan_types.contains(chan[0]))
								{
									auto nick = getSender(line);
									if (nick.isNotEmpty())
									{
										Jupiter::ReferenceString modestring = Jupiter::ReferenceString::substring(line, line.find(' ', 2) + 1);
										if (modestring.wordCount(" ") > 1)
										{
											Jupiter::ReferenceString modes = modestring.getWord(0, " ");
											if (modes.isNotEmpty())
											{
												Jupiter::ReferenceString modeParameters = modestring.gotoWord(1, " ");
												Jupiter::ReferenceString tword;
												unsigned char word_index = 0;
												char symb = 0;
												for (uint8_t mode_index = 0; mode_index != modes.size(); mode_index++)
												{
													if (modes[mode_index] == '+' || modes[mode_index] == '-')
														symb = modes[mode_index];
													else if (m_prefix_modes.contains(modes[mode_index])) // user prefix mode
													{
														tword = modeParameters.getWord(word_index, " ");
														if (tword.isNotEmpty() && tword[0] == ':') {
															// Sttrip leading ':'
															tword.shiftRight(1);

															// erase modeParameters
															modeParameters.erase();
														}

														if (tword.isNotEmpty())
														{
															Jupiter::IRC::Client::Channel *channel = getChannel(chan);
															if (channel != nullptr)
															{
																if (symb == '+')
																	channel->addUserPrefix(tword, m_prefixes[m_prefix_modes.find(modes[mode_index])]);
																else
																	channel->delUserPrefix(tword, m_prefixes[m_prefix_modes.find(modes[mode_index])]);
															}
														}
														++word_index;
													}
													else if (m_modeA.contains(modes[mode_index])) // mode type A
														++word_index;
													else if (m_modeB.contains(modes[mode_index])) // mode type B
														++word_index;
													else if (m_modeC.contains(modes[mode_index]) && symb == '+') // mode type C (with parameter)
														++word_index;
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
						else if (numeric == Reply::NAMREPLY) // Some names.
						{
							Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(line, 4, " ");
							Jupiter::ReferenceString names = Jupiter::ReferenceString::substring(line, line.find(':', 1) + 1);

							Channel *channel = getChannel(chan);
							if (channel != nullptr)
							{
								if (channel->m_adding_names == false)
								{
									Client::delChannel(chan);
									Client::addChannel(chan);
									channel = Jupiter::IRC::Client::getChannel(chan);
									channel->m_adding_names = true;
								}

								// addNamesToChannel can be cut/pasted here
								Client::addNamesToChannel(*channel, names);
							}
						}
						else if (numeric == Reply::ENDOFNAMES) // We're done here.
						{
							Jupiter::ReferenceString chan = Jupiter::ReferenceString::getWord(line, 3, " ");
							Channel *channel = getChannel(chan);

							if (channel != nullptr)
								channel->m_adding_names = false;
						}
						break;
					}
				}
			}
			else
			{
				if (w1.equals("PING"))
				{
					m_socket->send(Jupiter::StringS::Format("PONG %.*s" ENDL, w2.size(), w2.ptr()));
				}
				else if (w1.equals("NICK"))
				{
					if (w2.isNotEmpty())
						m_nickname = w2;
				}
				else if (w1.equals("ERROR"))
				{
					Jupiter::ReferenceString reason = Jupiter::ReferenceString::substring(line, line.find(':') + 1);
					this->OnError(reason);
					for (size_t i = 0; i < Jupiter::plugins->size(); i++)
						Jupiter::plugins->get(i)->OnError(this, reason);
					Jupiter::IRC::Client::disconnect();
				}
				else if (w1.equals("AUTHENTICATE"))
				{
					if (m_sasl_password.isNotEmpty())
					{
						Jupiter::StringS auth_str = m_nickname + '\0' + m_sasl_account + '\0' + m_sasl_password;

						char *enc = Jupiter::base64encode(auth_str.ptr(), auth_str.size());
						m_socket->send("AUTHENTICATE "_jrs + enc + ENDL);
						delete[] enc;
					}
					m_socket->send("CAP END" ENDL);
					Client::registerClient();
				}
			}
			if (numeric != 0)
			{
				this->OnNumeric(numeric, line);
				for (size_t i = 0; i < Jupiter::plugins->size(); i++)
					Jupiter::plugins->get(i)->OnNumeric(this, numeric, line);
			}
		}
		this->OnRaw(line);
		for (size_t i = 0; i < Jupiter::plugins->size(); i++)
			Jupiter::plugins->get(i)->OnRaw(this, line);
	}

	return 0;
}

bool Jupiter::IRC::Client::connect()
{
	const Jupiter::ReadableString &clientAddress = Jupiter::IRC::Client::readConfigValue("ClientAddress"_jrs);
	if (m_socket->connect(m_server_hostname.c_str(), m_server_port, clientAddress.isEmpty() ? nullptr : static_cast<std::string>(clientAddress).c_str(), (unsigned short)Jupiter::IRC::Client::readConfigLong("ClientPort"_jrs)) == false)
		return false;

	m_socket->setBlocking(false);
	if (m_ssl == false && Jupiter::IRC::Client::readConfigBool("STARTTLS"_jrs, true))
	{
		m_socket->send("STARTTLS" ENDL);
		m_connection_status = 1;
	}
	else
		Client::startCAP();

	return true;
}

void Jupiter::IRC::Client::disconnect(bool stayDead)
{
	m_connection_status = 0;
	m_socket->close();
	m_reconnect_time = time(0) + m_reconnect_delay;
	m_dead = stayDead;
	this->OnDisconnect();
	bool ssl = Jupiter::IRC::Client::readConfigBool("SSL"_jrs);
	if (ssl != m_ssl)
	{
		m_ssl = ssl;
		if (m_ssl)
		{
			Jupiter::SecureTCPSocket *t = new Jupiter::SecureTCPSocket(std::move(*m_socket));
			if (m_ssl_certificate.isNotEmpty())
				t->setCertificate(m_ssl_certificate, m_ssl_key);

			delete m_socket;
			m_socket = t;
		}
		else
		{
			Jupiter::TCPSocket *t = new Jupiter::TCPSocket(std::move(*m_socket));
			delete m_socket;
			m_socket = t;
		}
	}
	for (size_t i = 0; i < Jupiter::plugins->size(); i++)
		Jupiter::plugins->get(i)->OnDisconnect(this);
}

void Jupiter::IRC::Client::disconnect(const Jupiter::ReadableString &message, bool stayDead)
{
	m_socket->send(Jupiter::StringS::Format("QUIT :%.*s" ENDL, message.size(), message.ptr()));
	Jupiter::IRC::Client::disconnect(stayDead);
}

void Jupiter::IRC::Client::reconnect()
{
	if (m_connection_status != 0) Jupiter::IRC::Client::disconnect();
	m_reconnect_attempts++;
	bool successConnect = Jupiter::IRC::Client::connect();
	this->OnReconnectAttempt(successConnect);
	for (size_t i = 0; i < Jupiter::plugins->size(); i++)
		Jupiter::plugins->get(i)->OnReconnectAttempt(this, successConnect);
}

int Jupiter::IRC::Client::think()
{
	auto handle_error = [this](int error_code)
	{
		if (this->m_dead == true)
			return error_code;

		if (this->m_max_reconnect_attempts < 0 || this->m_reconnect_attempts < this->m_max_reconnect_attempts)
		{
			if (!this->m_reconnect_delay || this->m_reconnect_time < time(0))
				this->reconnect();

			return 0;
		}

		return error_code;
	};

	if (m_connection_status == 0)
		return handle_error(-1);

	int tmp = m_socket->recv();
	if (tmp > 0)
	{
		// Process incoming data
		Jupiter::ReadableString::TokenizeResult<Jupiter::Reference_String> result = Jupiter::ReferenceString::tokenize(m_socket->getBuffer(), "\r\n"_jrs);
		if (result.token_count != 0)
		{
			if (result.tokens[0].size() > 0)
			{
				// Ensure there's not a token getting split over separate buffers
				if (m_last_line.size() > 0)
				{
					if (result.tokens[0][0] == '\n' && m_last_line[m_last_line.size() - 1] == '\r')
					{
						m_last_line.truncate(1); // Remove \r

						Jupiter::IRC::Client::process_line(m_last_line);
						m_last_line.erase();

						result.tokens[0].shiftRight(1); // Remove \n
					}
				}

				m_last_line += result.tokens[0];
			}

			if (result.token_count != 1)
			{
				Jupiter::IRC::Client::process_line(m_last_line);
				m_last_line = result.tokens[result.token_count - 1];

				for (size_t index = 1; index != result.token_count - 1; ++index)
					if (Jupiter::IRC::Client::process_line(result.tokens[index]) != 0)
						return handle_error(1);
			}
		}

		return 0;
	}

	// No incoming data; check for errors
	tmp = m_socket->getLastError();

	if (tmp == JUPITER_SOCK_EWOULDBLOCK) // Operation would block
		return 0;

	// Serious error; disconnect if necessary
	if (m_connection_status != 0)
		Jupiter::IRC::Client::disconnect();

	return handle_error(tmp);
}

const Jupiter::ReadableString &Jupiter::IRC::Client::readConfigValue(const Jupiter::ReadableString &key, const Jupiter::ReadableString &defaultValue) const
{
	if (m_primary_section != nullptr)
	{
		const Jupiter::ReadableString &val = m_primary_section->get(key);

		if (val.isNotEmpty())
			return val;
	}

	if (m_secondary_section != nullptr)
		return m_secondary_section->get(key, defaultValue);

	return defaultValue;
}

bool Jupiter::IRC::Client::readConfigBool(const Jupiter::ReadableString &key, bool defaultValue) const
{
	if (m_primary_section != nullptr)
	{
		const Jupiter::ReadableString &val = m_primary_section->get(key);

		if (val.isNotEmpty())
			return val.asBool();
	}

	if (m_secondary_section != nullptr)
		return m_secondary_section->get<bool>(key, defaultValue);

	return defaultValue;
}

int Jupiter::IRC::Client::readConfigInt(const Jupiter::ReadableString &key, int defaultValue) const
{
	if (m_primary_section != nullptr)
	{
		const Jupiter::ReadableString &val = m_primary_section->get(key);

		if (val.isNotEmpty())
			return val.asInt();
	}

	if (m_secondary_section != nullptr)
		return m_secondary_section->get<int>(key, defaultValue);

	return defaultValue;
}

long Jupiter::IRC::Client::readConfigLong(const Jupiter::ReadableString &key, long defaultValue) const
{
	if (m_primary_section != nullptr)
	{
		const Jupiter::ReadableString &val = m_primary_section->get(key);

		if (val.isNotEmpty())
			return val.asInt();
	}

	if (m_secondary_section != nullptr)
		return m_secondary_section->get<long>(key, defaultValue);

	return defaultValue;
}

double Jupiter::IRC::Client::readConfigDouble(const Jupiter::ReadableString &key, double defaultValue) const
{
	if (m_primary_section != nullptr)
	{
		const Jupiter::ReadableString &val = m_primary_section->get(key);

		if (val.isNotEmpty())
			return val.asDouble();
	}

	if (m_secondary_section != nullptr)
		return m_secondary_section->get<double>(key, defaultValue);

	return defaultValue;
}

void Jupiter::IRC::Client::writeToLogs(const Jupiter::ReadableString &message)
{
	if (m_log_file != nullptr)
	{
		message.println(m_log_file);
		fflush(m_log_file);
	}
}

/**
* @brief IRC Client Private Function Implementations
*/

void Jupiter::IRC::Client::delChannel(const Jupiter::ReadableString &in_channel)
{
	m_channels.erase(in_channel);
}

std::shared_ptr<Jupiter::IRC::Client::User> Jupiter::IRC::Client::findUser(const Jupiter::ReadableString &in_nickname) const
{
	return getUser(in_nickname);
}

std::shared_ptr<Jupiter::IRC::Client::User> Jupiter::IRC::Client::findUserOrAdd(const Jupiter::ReadableString &name)
{
	Jupiter::ReferenceString nick = Jupiter::ReferenceString::getWord(name, 0, "!");
	auto result = Jupiter::IRC::Client::findUser(nick);

	if (result != nullptr)
		return result;

	auto user = std::make_shared<User>();
	user->m_nickname = nick;
	user->m_username = Jupiter::ReferenceString::getWord(name, 1, "!@");
	user->m_hostname = Jupiter::ReferenceString::getWord(name, 2, "!@");
	return m_users.emplace(nick, user).first->second;
}

void Jupiter::IRC::Client::addNamesToChannel(Channel &in_channel, Jupiter::ReadableString &in_names)
{
	Jupiter::ReferenceString tmp;
	size_t offset;
	unsigned int nameLen = in_names.wordCount(" ");

	for (size_t i = 0; i != nameLen; i++)
	{
		tmp = Jupiter::ReferenceString::getWord(in_names, i, " ");

		if (tmp.isNotEmpty())
		{
			offset = tmp.span(m_prefixes);
			tmp.shiftRight(offset);

			auto user = Client::findUserOrAdd(tmp);
			tmp.shiftLeft(offset);

			in_channel.addUser(user);

			while (offset > 0)
			{
				--offset;

				in_channel.addUserPrefix(user->getNickname(), tmp[offset]);
			}
		}
	}
}

void Jupiter::IRC::Client::addChannel(const Jupiter::ReadableString &in_channel)
{
	m_channels.emplace(in_channel, Channel(in_channel, this));
}

bool Jupiter::IRC::Client::startCAP()
{
	m_connection_status = 2;
	return m_socket->send("CAP LS"_jrs ENDL) > 0;
}

bool Jupiter::IRC::Client::registerClient()
{
	bool result = true;
	const char *localHostname = Jupiter::Socket::getLocalHostname();
	Jupiter::StringS messageToSend;

	messageToSend.format("USER %.*s %s %.*s :%.*s" ENDL, m_nickname.size(), m_nickname.ptr(), localHostname, m_server_hostname.size(), m_server_hostname.c_str(), m_realname.size(), m_realname.ptr());
	
	if (m_socket->send(messageToSend) <= 0)
		result = false;

	messageToSend.format("NICK %.*s" ENDL, m_nickname.size(), m_nickname.ptr());

	if (m_socket->send(messageToSend) <= 0)
		result = false;

	m_connection_status = 3;
	return result;
}

/**
* User Implementation
*/

const Jupiter::ReadableString &Jupiter::IRC::Client::User::getNickname() const
{
	return m_nickname;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::User::getUsername() const
{
	return m_username;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::User::getHostname() const
{
	return m_hostname;
}

size_t Jupiter::IRC::Client::User::getChannelCount() const
{
	return m_channel_count;
}

/**
* Channel Implementation
*/

Jupiter::IRC::Client::Channel::Channel(const Jupiter::ReadableString &in_name, Jupiter::IRC::Client *in_parent)
{
	auto to_lower = [&in_name]()
	{
		Jupiter::String result(in_name.size());
		const char *itr = in_name.ptr();
		const char *end = itr + in_name.size();

		while (itr != end)
		{
			result += static_cast<char>(tolower(*itr));
			++itr;
		}

		return result;
	};

	m_name = in_name;
	m_parent = in_parent;
	m_type = m_parent->getDefaultChanType();

	Jupiter::String name = to_lower();

	auto read_type = [&name](Jupiter::Config &in_config, int default_type)
	{
		return in_config["Channels"_jrs][name].get<int>("Type"_jrs, default_type);
	};

	if (m_parent->getSecondaryConfigSection() != nullptr)
		m_type = read_type(*m_parent->getSecondaryConfigSection(), m_type);

	if (m_parent->getPrimaryConfigSection() != nullptr)
		m_type = read_type(*m_parent->getPrimaryConfigSection(), m_type);
}

std::shared_ptr<Jupiter::IRC::Client::Channel::User> Jupiter::IRC::Client::Channel::addUser(std::shared_ptr<Client::User> user)
{
	auto channel_user = std::make_shared<Channel::User>();
	channel_user->m_user = user;

	++user->m_channel_count;

	m_users[channel_user->getNickname()] = channel_user;
	return channel_user;
}

std::shared_ptr<Jupiter::IRC::Client::Channel::User> Jupiter::IRC::Client::Channel::addUser(std::shared_ptr<Client::User> user, const char prefix)
{
	auto channel_user = std::make_shared<Channel::User>();
	channel_user->m_user = user;
	channel_user->m_prefixes = prefix;

	++user->m_channel_count;

	m_users[channel_user->getNickname()] = channel_user;
	return channel_user;
}

void Jupiter::IRC::Client::Channel::delUser(const Jupiter::ReadableString &in_nickname)
{
	m_users.erase(in_nickname);
}

void Jupiter::IRC::Client::Channel::addUserPrefix(const Jupiter::ReadableString &in_nickname, char prefix)
{
	auto user = getUser(in_nickname);

	if (user != nullptr && user->m_prefixes.contains(prefix) == false)
		user->m_prefixes += prefix;
}

void Jupiter::IRC::Client::Channel::delUserPrefix(const Jupiter::ReadableString &in_nickname, char prefix)
{
	auto user = getUser(in_nickname);

	if (user != nullptr)
		user->m_prefixes.remove(prefix);
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::getName() const
{
	return m_name;
}

std::shared_ptr<Jupiter::IRC::Client::Channel::User> Jupiter::IRC::Client::Channel::getUser(const Jupiter::ReadableString &in_nickname) const
{
	auto user = m_users.find(in_nickname);
	if (user != m_users.end()) {
		return user->second;
	}

	return nullptr;
}

char Jupiter::IRC::Client::Channel::getUserPrefix(const Channel::User &in_user) const
{
	const Jupiter::ReadableString &prefixes = m_parent->getPrefixes();

	for (size_t index = 0; index != prefixes.size(); ++index)
		if (in_user.m_prefixes.contains(prefixes[index]))
			return prefixes[index];

	return 0;
}

char Jupiter::IRC::Client::Channel::getUserPrefix(const Jupiter::ReadableString &in_nickname) const
{
	auto user = getUser(in_nickname);

	if (user != nullptr)
		return this->getUserPrefix(*user);

	return 0;
}

const Jupiter::IRC::Client::Channel::UserTableType &Jupiter::IRC::Client::Channel::getUsers() const
{
	return m_users;
}

size_t Jupiter::IRC::Client::Channel::getUserCount() const
{
	return m_users.size();
}

int Jupiter::IRC::Client::Channel::getType() const
{
	return m_type;
}

void Jupiter::IRC::Client::Channel::setType(int in_type)
{
	m_type = in_type;
}

/**
* Channel User Implementation
*/

Jupiter::IRC::Client::Channel::User::~User()
{
	if (m_user != nullptr)
		--m_user->m_channel_count;
}

Jupiter::IRC::Client::User *Jupiter::IRC::Client::Channel::User::getUser() const
{
	return m_user.get();
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::User::getPrefixes() const
{
	return m_prefixes;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::User::getNickname() const
{
	return m_user->getNickname();
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::User::getUsername() const
{
	return m_user->getUsername();
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::User::getHostname() const
{
	return m_user->getHostname();
}

size_t Jupiter::IRC::Client::Channel::User::getChannelCount() const
{
	return m_user->getChannelCount();
}