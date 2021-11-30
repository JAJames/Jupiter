/**
 * Copyright (C) 2013-2021 Jessica James.
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

#include "IRC_Client.h"
#include <cstring>
#include <cstdio>
#include <ctime>
#include <charconv>
#include "jessilib/split.hpp"
#include "jessilib/word_split.hpp"
#include "jessilib/unicode.hpp"
#include "Jupiter.h"
#include "Functions.h"
#include "TCPSocket.h"
#include "String.hpp"
#include "Plugin.h"
#include "Base64.h"

//#define SHORT_IRC_MACROS
#include "IRC_Numerics.h"

using namespace Jupiter::literals;
using namespace std::literals;

Jupiter::IRC::Client::Client(Jupiter::Config *in_primary_section, Jupiter::Config *in_secondary_section) {
	m_primary_section = in_primary_section;
	m_secondary_section = in_secondary_section;

	if (m_primary_section != nullptr)
		m_primary_section_name = m_primary_section->getName();

	m_server_hostname = static_cast<std::string>(Jupiter::IRC::Client::readConfigValue("Hostname"_jrs, ""_jrs));
	
	m_log_file_name = static_cast<std::string>(Jupiter::IRC::Client::readConfigValue("LogFile"_jrs));
	m_nickname = Jupiter::IRC::Client::readConfigValue("Nick"_jrs, "Jupiter"_jrs);

	m_realname = Jupiter::IRC::Client::readConfigValue("RealName"_jrs, "Jupiter IRC Client"_jrs);

	m_sasl_password = Jupiter::IRC::Client::readConfigValue("SASL.Password"_jrs);
	if (m_sasl_password.empty())
		m_sasl_password = Jupiter::IRC::Client::readConfigValue("SASL.Pass"_jrs);
	
	m_sasl_account = Jupiter::IRC::Client::readConfigValue("SASL.Account"_jrs);
	if (m_sasl_account.empty())
		m_sasl_account = m_nickname;

	m_auto_part_message = Jupiter::IRC::Client::readConfigValue("AutoPartMessage"_jrs);

	m_ssl = Jupiter::IRC::Client::readConfigBool("SSL"_jrs);
	m_ssl_certificate = Jupiter::IRC::Client::readConfigValue("Certificate"_jrs);
	m_ssl_key = Jupiter::IRC::Client::readConfigValue("Key"_jrs);
	if (m_ssl_certificate.empty())
	{
		m_ssl_certificate = Jupiter::IRC::Client::readConfigValue("Cert"_jrs);
		if (m_ssl_certificate.empty())
			m_ssl_certificate = m_ssl_key;
	}
	if (m_ssl_key.empty())
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

	if (m_ssl) {
		Jupiter::SecureTCPSocket *t = new Jupiter::SecureTCPSocket();

		if (!m_ssl_certificate.empty())
			t->setCertificate(m_ssl_certificate, m_ssl_key);

		m_socket.reset(t);
	}
	else {
		m_socket.reset(new Jupiter::TCPSocket());
	}

	m_connection_status = 0;
}

Jupiter::IRC::Client::~Client() {
	if (m_socket != nullptr) {
		m_socket->close();
		m_socket = nullptr;
	}

	if (m_log_file != nullptr)
		fclose(m_log_file);
}

void Jupiter::IRC::Client::OnConnect(){
}

void Jupiter::IRC::Client::OnDisconnect(){
}

void Jupiter::IRC::Client::OnReconnectAttempt(bool){
}

void Jupiter::IRC::Client::OnRaw(std::string_view){
}

void Jupiter::IRC::Client::OnNumeric(long int, std::string_view){
}

void Jupiter::IRC::Client::OnError(std::string_view){
}

void Jupiter::IRC::Client::OnChat(std::string_view, std::string_view, std::string_view){
}

void Jupiter::IRC::Client::OnNotice(std::string_view, std::string_view, std::string_view){
}

void Jupiter::IRC::Client::OnServerNotice(std::string_view, std::string_view, std::string_view){
}

void Jupiter::IRC::Client::OnCTCP(std::string_view, std::string_view, std::string_view, std::string_view) {
}

void Jupiter::IRC::Client::OnAction(std::string_view, std::string_view, std::string_view) {
}

void Jupiter::IRC::Client::OnInvite(std::string_view, std::string_view, std::string_view) {
}

void Jupiter::IRC::Client::OnJoin(std::string_view, std::string_view) {
}

void Jupiter::IRC::Client::OnPart(std::string_view, std::string_view, std::string_view) {
}

void Jupiter::IRC::Client::OnNick(std::string_view, std::string_view) {
}

void Jupiter::IRC::Client::OnKick(std::string_view, std::string_view, std::string_view, std::string_view) {
}

void Jupiter::IRC::Client::OnQuit(std::string_view, std::string_view) {
}

void Jupiter::IRC::Client::OnMode(std::string_view, std::string_view, std::string_view) {
}

std::string_view Jupiter::IRC::Client::getConfigSection() const {
	if (m_primary_section != nullptr) {
		return m_primary_section_name;
	}

	return ""_jrs;
}

Jupiter::Config *Jupiter::IRC::Client::getPrimaryConfigSection() const {
	return m_primary_section;
}

Jupiter::Config *Jupiter::IRC::Client::getSecondaryConfigSection() const {
	return m_secondary_section;
}

void Jupiter::IRC::Client::setPrimaryConfigSection(Jupiter::Config *in_primary_section) {
	m_primary_section = in_primary_section;

	if (m_primary_section != nullptr)
		m_primary_section_name = m_primary_section->getName();
	else
		m_primary_section_name.erase();
}

void Jupiter::IRC::Client::setSecondaryConfigSection(Jupiter::Config *in_secondary_section) {
	m_secondary_section = in_secondary_section;
}

const std::string &Jupiter::IRC::Client::getLogFile() const {
	return m_log_file_name;
}

std::string_view Jupiter::IRC::Client::getPrefixes() const {
	return m_prefixes;
}

std::string_view Jupiter::IRC::Client::getPrefixModes() const {
	return m_prefix_modes;
}

std::string_view Jupiter::IRC::Client::getNickname() const {
	return m_nickname;
}

std::string_view Jupiter::IRC::Client::getRealname() const {
	return m_realname;
}

const Jupiter::ReadableString &Jupiter::IRC::Client::getServerName() const {
	return m_server_name;
}

const std::string &Jupiter::IRC::Client::getServerHostname() const {
	return m_server_hostname;
}

unsigned short Jupiter::IRC::Client::getServerPort() const {
	return m_server_port;
}

time_t Jupiter::IRC::Client::getReconnectDelay() const {
	return m_reconnect_delay;
}

time_t Jupiter::IRC::Client::getReconnectTime() const {
	return m_reconnect_time;
}

int Jupiter::IRC::Client::getReconnectAttempts() const {
	return m_reconnect_attempts;
}

int Jupiter::IRC::Client::getMaxReconnectAttempts() const {
	return m_max_reconnect_attempts;
}

int Jupiter::IRC::Client::getDefaultChanType() const {
	return m_default_chan_type;
}

FILE *Jupiter::IRC::Client::getPrintOutput() const {
	return m_output;
}

void Jupiter::IRC::Client::setPrintOutput(FILE *f) {
	m_output = f;
}

Jupiter::ReferenceString getSender(std::string_view line) {
	return jessilib::word_split_once_view(line, ":! "sv).first;
}

int Jupiter::IRC::Client::getAccessLevel(const Channel &in_channel, std::string_view in_nickname) const {
	char prefix = in_channel.getUserPrefix(in_nickname);

	if (prefix != 0) {
		return static_cast<int>(m_prefixes.size() - m_prefixes.find(prefix));
	}

	return 0;
}

int Jupiter::IRC::Client::getAccessLevel(std::string_view in_channel, std::string_view in_nickname) const {
	auto channel = m_channels.find(Jupiter::ReferenceString{in_channel});

	if (channel != m_channels.end())
		return this->getAccessLevel(channel->second, in_nickname);

	return 0;
}

void Jupiter::IRC::Client::send(const Jupiter::ReadableString &rawMessage) {
	Jupiter::StringS out = rawMessage;
	out += ENDL;

	m_socket->send(out);
}

const Jupiter::IRC::Client::UserTableType &Jupiter::IRC::Client::getUsers() const {
	return m_users;
}

size_t Jupiter::IRC::Client::getUserCount() const {
	return m_users.size();
}

std::shared_ptr<Jupiter::IRC::Client::User> Jupiter::IRC::Client::getUser(std::string_view in_nickname) const {
	auto user = m_users.find(Jupiter::ReferenceString{in_nickname});
	if (user != m_users.end()) {
		return user->second;
	}

	return nullptr;
}

const Jupiter::IRC::Client::ChannelTableType &Jupiter::IRC::Client::getChannels() const {
	return m_channels;
}

size_t Jupiter::IRC::Client::getChannelCount() const {
	return m_channels.size();
}

Jupiter::IRC::Client::Channel *Jupiter::IRC::Client::getChannel(std::string_view in_channel) const {
	Jupiter::ReferenceString channel_name = in_channel; // TODO: remove this
	auto channel = m_channels.find(channel_name);
	if (channel != m_channels.end()) {
		return const_cast<Channel*>(&channel->second);
	}

	return nullptr;
}

bool Jupiter::IRC::Client::isAutoReconnect() const {
	return m_max_reconnect_attempts != 0;
}

void Jupiter::IRC::Client::setAutoReconnect(int val) {
	m_max_reconnect_attempts = val;
}

void Jupiter::IRC::Client::joinChannel(std::string_view in_channel) {
	m_socket->send(Jupiter::StringS::Format("JOIN %.*s" ENDL, in_channel.size(), in_channel.data()));
}

void Jupiter::IRC::Client::joinChannel(std::string_view in_channel, std::string_view in_password) {
	m_socket->send(Jupiter::StringS::Format("JOIN %.*s %.*s" ENDL, in_channel.size(), in_channel.data(), in_password.size(), in_password.data()));
}

void Jupiter::IRC::Client::partChannel(std::string_view in_channel) {
	m_socket->send(Jupiter::StringS::Format("PART %.*s" ENDL, in_channel.size(), in_channel.data()));

	Jupiter::ReferenceString channel_name = in_channel;
	m_channels[channel_name].setType(-2);
}

void Jupiter::IRC::Client::partChannel(std::string_view in_channel, std::string_view in_message) {
	m_socket->send(Jupiter::StringS::Format("PART %.*s :%.*s" ENDL, in_channel.size(), in_channel.data(), in_message.size(), in_message.data()));

	Jupiter::ReferenceString channel_name = in_channel;
	m_channels[channel_name].setType(-2);
}

void Jupiter::IRC::Client::sendMessage(std::string_view dest, std::string_view message) {
	m_socket->send(Jupiter::StringS::Format("PRIVMSG %.*s :%.*s" ENDL, dest.size(), dest.data(), message.size(), message.data()));
}

void Jupiter::IRC::Client::sendNotice(std::string_view dest, std::string_view message) {
	m_socket->send(Jupiter::StringS::Format("NOTICE %.*s :%.*s" ENDL, dest.size(), dest.data(), message.size(), message.data()));
}

size_t Jupiter::IRC::Client::messageChannels(int type, std::string_view message) {
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

int Jupiter::IRC::Client::process_line(std::string_view in_line) {
	Jupiter::ReferenceString line{in_line}; // TODO: remove this
	if (line.isNotEmpty())
	{
		Jupiter::IRC::Client::writeToLogs(line);
		if (m_output != nullptr) {
			// TODO: use ostream instead
			fwrite(in_line.data(), sizeof(char), in_line.size(), m_output);
			fputs("\r\n", m_output);
		}

		auto first_split = jessilib::split_once_view(line, " "sv);
		Jupiter::ReferenceString w1 = first_split.first;
		if (w1.isNotEmpty())
		{
			int numeric = asInt(first_split.second);
			if (w1[0] == ':') { //Messages
				if (!first_split.second.empty()) {
					// TODO: This entire method should basically just be a state machine instead of this massive mess
					auto line_split = jessilib::split_view(line, " "sv);
					auto getLineToken = [&line_split](size_t index) -> Jupiter::ReferenceString {
						if (index < line_split.size()) {
							return line_split[index];
						}

						return {};
					};

					switch (numeric) // Numerics that don't rely on a specific connectionStatus.
					{
					case Reply::BOUNCE: // 010
					{
						std::string_view portToken = getLineToken(4);
						unsigned short port{};
						if (!portToken.empty() && portToken[0] == '+') { // This is most likely not used anywhere.
							portToken.remove_prefix(1);
							if (m_ssl == false) {
								m_ssl = true;
								m_socket.reset(new Jupiter::SecureTCPSocket());
							}
						}
						else {
							if (m_ssl == true) {
								m_ssl = false;
								m_socket.reset(new Jupiter::TCPSocket());
							}
						}

						std::from_chars(portToken.data(), portToken.data() + portToken.size(), port, 10);
						if (port != 0) // Don't default -- could be non-compliant input.
						{
							m_server_hostname = getLineToken(3);
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
						case Error::UNKNOWNCOMMAND: // 421
						{
							Jupiter::ReferenceString command = getLineToken(2);
							if (command.equalsi("STARTTLS")) { // Server doesn't support STARTTLS
								Client::startCAP();
							}
						}
						break;

						case Reply::STARTTLS: // 670
						{
							Jupiter::SecureTCPSocket *t = new Jupiter::SecureTCPSocket(std::move(*m_socket));
							m_socket.reset(t);
							m_ssl = true;
							// toggle blocking to prevent error
							if (!m_ssl_certificate.empty())
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
							if (getLineToken(1).equalsi("CAP"))
							{
								Jupiter::ReferenceString w4 = getLineToken(3);
								if (w4 == "LS"sv)
								{
									std::vector<std::string_view> cap_list{ line_split.begin() + 4, line_split.end() };
									if (!cap_list.empty() && !cap_list.front().empty() && cap_list.front().front() == ':') {
										cap_list.front().remove_prefix(1);
									}
									std::string caps_request = "CAP REQ :";
									bool sasl = false;
									for (const auto& cap : cap_list) {
										if (jessilib::equalsi(cap, "multi-prefix"sv)) {
											caps_request += cap;
											caps_request.push_back(' ');
										}
										else if (jessilib::equalsi(cap, "userhost-in-names"sv)) {
											caps_request += cap;
											caps_request.push_back(' ');
										}
										else if (jessilib::equalsi(cap, "sasl"sv)) {
											if (!m_sasl_password.empty()) {
												caps_request += cap;
												caps_request.push_back(' ');
												sasl = true;
											}
										}
										// else; // We don't know what this is!
									}
									if (caps_request.size() > 9)
									{
										caps_request.pop_back();
										caps_request += ENDL;
										m_socket->send(caps_request);
										if (sasl) {
											m_socket->send("AUTHENTICATE PLAIN"sv ENDL);
										}
									}
									if (!sasl)
									{
										m_socket->send("CAP END"sv ENDL);
										Client::registerClient();
									}
								}
							}
							break;
						case Error::UNKNOWNCOMMAND: // 421
							if (jessilib::equalsi(first_split.second, "CAP"sv)) { // Server doesn't support CAP
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
							m_server_name = getLineToken(3);
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
							std::string_view altNick = Jupiter::IRC::Client::readConfigValue("AltNick"_jrs);
							std::string_view configNick = Jupiter::IRC::Client::readConfigValue("Nick"_jrs, "Jupiter"_jrs);

							if (!altNick.empty() && jessilib::equalsi(m_nickname, altNick)) // The alternate nick failed.
							{
								m_nickname = configNick;
								m_nickname += "1";
								
								m_socket->send("NICK "_jrs + m_nickname + ENDL);
							}
							else if (jessilib::equalsi(m_nickname, configNick)) // The config nick failed
							{
								if (altNick.empty())
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
										int n = Jupiter_strtoi_nospace_s(m_nickname.data() + configNick.size(), m_nickname.size() - configNick.size(), 10);
										m_nickname = configNick;
										m_nickname += std::to_string(n + 1);

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
							// Parse supported user prefixes
							size_t pos = line.find("PREFIX=("_jrs);
							if (pos != Jupiter::INVALID_INDEX)
							{
								std::string_view prefix_line_start = line.substr(pos + 8);
								size_t prefix_modes_end = prefix_line_start.find(')');
								if (prefix_modes_end != std::string_view::npos) {
									m_prefix_modes = prefix_line_start.substr(0, prefix_modes_end);
									prefix_line_start.remove_prefix(m_prefix_modes.size() + 1);
									m_prefixes = jessilib::word_split_once_view(prefix_line_start, " "sv).first;
								}
							}

							// Parse supported channel modes
							pos = line.find("CHANMODES="_jrs);
							if (pos != Jupiter::INVALID_INDEX) {
								std::string_view chan_modes_view = line.substr(pos + 10, line.find(' '));
								std::vector<std::string_view> chan_modes = jessilib::split_n_view(chan_modes_view, ","sv, 3); // only split 3 times to cover A-D, but server _can_ send more
								if (chan_modes.size() > 0) {
									m_modeA = chan_modes[0];
									if (chan_modes.size() > 1) {
										m_modeB = chan_modes[1];
										if (chan_modes.size() > 2) {
											m_modeC = chan_modes[2];
											if (chan_modes.size() > 3) {
												m_modeD = chan_modes[3];
											}
										}
									}
								}
							}

							// Parse supported channel types
							pos = line.find("CHANTYPES="_jrs);
							if (pos != Jupiter::INVALID_INDEX) {
								m_chan_types = line.substr(pos + 10, line.find(' '));
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
								return !value.empty();
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
							for (auto& plugin : Jupiter::plugins) {
								plugin->OnConnect(this);
							}
						}
						break;
						}
						break;

					default: { // Post-registration.
						std::string_view command_token = getLineToken(1);
						if (jessilib::equalsi(command_token, "PRIVMSG"sv)) {
							std::string_view channel_name = getLineToken(2);
							if (!channel_name.empty()) {
								std::string_view nick = getSender(line);
								if (!nick.empty()) {
									std::string_view message_view = line.substr(std::min(line.find(':', 1), line.size()));
									if (!message_view.empty()) {
										message_view.remove_prefix(1);
									}
									if (!message_view.empty() && message_view.front() == Jupiter::IRC::CTCP) { //CTCP (ACTIONs are included)
										// Strip leading & trailing CTCP tokens
										message_view.remove_prefix(1);
										message_view = message_view.substr(0, message_view.find(Jupiter::IRC::CTCP));

										auto split_message = jessilib::split_once_view(message_view, WHITESPACE_SV);
										std::string_view ctcp_command = split_message.first;
										std::string_view ctcp_parameters = split_message.second;
										if (ctcp_command == "ACTION"sv) {
											this->OnAction(channel_name, nick, ctcp_parameters);
											for (auto& plugin: Jupiter::plugins) {
												plugin->OnAction(this, channel_name, nick, ctcp_parameters);
											}
										}
										else {
											std::string response = "NOTICE "s;
											response += nick;
											response += " :" IRCCTCP;
											response += ctcp_command;
											response += ' ';
											if (ctcp_command == "PING"sv)
												response += ctcp_parameters;
											else if (ctcp_command == "VERSION"sv)
												response += Jupiter::version;
											else if (ctcp_command == "FINGER"sv)
												response += Jupiter::version;
											else if (ctcp_command == "SOURCE"sv)
												response += "https://github.com/JAJames/Jupiter";
											else if (ctcp_command == "USERINFO"sv)
												response += "Hey, I'm Jupiter! If you have questions, ask Agent! (GitHub: JAJames; Discord: Agent#0001)";
											else if (ctcp_command == "CLIENTINFO"sv)
												response += "I'll tell you what I don't know: This command!";
											else if (ctcp_command == "TIME"sv)
												response += getTime();
											else if (ctcp_command == "ERRMSG"sv)
												response += ctcp_parameters;
											else {
												response = "NOTICE ";
												response += nick;
												response += " :" IRCCTCP "ERRMSG ";
												response += ctcp_command;
												response += " :Query is unknown";
											}
											response += IRCCTCP ENDL;
											m_socket->send(response);

											this->OnCTCP(channel_name, nick, ctcp_command, ctcp_parameters);
											for (auto& plugin: Jupiter::plugins) {
												plugin->OnCTCP(this, channel_name, nick, message_view);
											}
										}
									}
									else {
										Jupiter::ReferenceString message = message_view;
										this->OnChat(channel_name, nick, message);
										for (auto& plugin: Jupiter::plugins) {
											plugin->OnChat(this, channel_name, nick, message);
										}
									}
								}
							}
						}
						else if (jessilib::equalsi(command_token, "NOTICE"sv)) {
							std::string_view channel_name = getLineToken(2);
							if (!channel_name.empty()) {
								size_t pos = line.find('!', 0);
								auto message = jessilib::split_once_view(line, ':').second;
								if (pos < line.find(' ')) {
									auto nick = line.substr(1, pos);
									this->OnNotice(channel_name, nick, message);
									for (auto& plugin: Jupiter::plugins) {
										plugin->OnNotice(this, channel_name, nick, message);
									}
								}
								else {
									auto sender = getSender(line);
									if (sender.isNotEmpty()) {
										this->OnServerNotice(channel_name, sender, message);
										for (auto& plugin: Jupiter::plugins) {
											plugin->OnServerNotice(this, channel_name, sender, message);
										}
									}
								}
							}
						}
						else if (jessilib::equalsi(command_token, "NICK"sv)) {
							auto nick = getSender(line);
							std::string_view newnick = jessilib::split_once_view(line, ' ').second;
							if (!newnick.empty() && newnick[0] == ':') {
								newnick.remove_prefix(1);
							}
							if (nick.equalsi(m_nickname)) {
								m_nickname = newnick;
							}
							auto user = Client::findUser(nick);
							if (user != nullptr) {
								user->m_nickname = newnick;
								this->OnNick(nick, newnick);
							}
							for (auto& plugin: Jupiter::plugins) {
								plugin->OnNick(this, nick, newnick);
							}
						}
						else if (jessilib::equalsi(command_token, "JOIN"sv)) {
							auto nick = getSender(line);
							std::string_view channel_name = getLineToken(2);
							if (!channel_name.empty() && channel_name.front() == ':') {
								channel_name.remove_prefix(1);
							}

							auto channel = getChannel(channel_name);
							if (jessilib::equalsi(m_nickname, nick)) {
								// TODO: Optimize by simply wiping channel data, rather than removing and re-adding
								if (channel != nullptr)
									delChannel(channel->getName());

								addChannel(channel_name);
								channel = getChannel(channel_name);
								channel->m_adding_names = true;

								if (channel->getType() < 0) {
									if (!m_auto_part_message.empty())
										partChannel(channel_name, m_auto_part_message);
									else
										partChannel(channel_name);
								}
							}
							else if (channel != nullptr)
								channel->addUser(Client::findUserOrAdd(nick));

							this->OnJoin(channel_name, nick);

							for (auto& plugin: Jupiter::plugins) {
								plugin->OnJoin(this, channel_name, nick);
							}
						}
						else if (jessilib::equalsi(command_token, "PART"sv)) {
							auto nick = getSender(line);
							if (nick.isNotEmpty()) {
								std::string_view channel_name = getLineToken(2);
								if (!channel_name.empty()) {
									Channel* channel = getChannel(channel_name);
									if (channel != nullptr) {
										auto user = getUser(nick);
										if (user != nullptr) {
											channel->delUser(nick);
											Jupiter::ReferenceString reason;
											size_t pos = line.find(':', 1);

											if (pos != std::string_view::npos)
												reason = line.substr(pos + 1);

											this->OnPart(channel_name, nick, reason);

											for (auto& plugin: Jupiter::plugins) {
												plugin->OnPart(this, channel_name, nick, reason);
											}

											if (nick.equalsi(m_nickname))
												Client::delChannel(channel_name);

											if (user->getChannelCount() == 0)
												m_users.erase(nick);
										}
									}
								}
							}
						}
						else if (jessilib::equalsi(command_token, "KICK"sv)) {
							std::string_view channel_name = getLineToken(2);
							if (!channel_name.empty()) {
								Jupiter::ReferenceString kicker = getSender(line);
								if (kicker.isNotEmpty()) {
									std::string_view kicked_nickname = getLineToken(3);
									if (!kicked_nickname.empty()) {
										Channel* channel = getChannel(channel_name);
										if (channel != nullptr) {
											auto user = getUser(kicked_nickname);
											if (user != nullptr) {
												channel->delUser(kicked_nickname);
												size_t pos = line.find(':', 1);
												Jupiter::ReferenceString reason;

												if (pos != std::string_view::npos)
													reason = line.substr(pos + 1);

												this->OnKick(channel_name, kicker, kicked_nickname, reason);

												for (auto& plugin: Jupiter::plugins) {
													plugin->OnKick(this, channel_name, kicker, kicked_nickname, reason);
												}

												if (jessilib::equalsi(kicked_nickname, m_nickname)) {
													Client::delChannel(channel_name);
													if (m_join_on_kick) {
														Jupiter::IRC::Client::joinChannel(channel_name);
													}
												}

												if (user->getChannelCount() == 0) {
													m_users.erase(Jupiter::ReferenceString{kicked_nickname});
												}
											}
										}
									}
								}
							}
						}
						else if (jessilib::equalsi(command_token, "QUIT"sv)) {
							Jupiter::ReferenceString nick = getSender(line);
							std::string_view message = jessilib::split_once_view(line.substr(1), ':').second;
							auto user = getUser(nick);
							if (user != nullptr) {
								for (auto& channel: m_channels) {
									channel.second.delUser(nick);
								}

								this->OnQuit(nick, message);

								for (auto& plugin: Jupiter::plugins) {
									plugin->OnQuit(this, nick, message);
								}

								m_users.erase(nick);
							}
						}
						else if (jessilib::equalsi(command_token, "INVITE"sv)) {
							Jupiter::ReferenceString inviter = getSender(line);
							std::string_view invited_nickname = getLineToken(2);
							std::string_view channel_name = jessilib::split_once_view(line.substr(1), ':').second;
							this->OnInvite(channel_name, inviter, invited_nickname);
							for (auto& plugin: Jupiter::plugins) {
								plugin->OnInvite(this, channel_name, inviter, invited_nickname);
							}
						}
						else if (jessilib::equalsi(command_token, "MODE"sv)) {
							std::string_view channel_name = getLineToken(2);
							if (!channel_name.empty()) {
								if (m_chan_types.find(channel_name[0]) != std::string::npos) {
									auto nick = getSender(line);
									if (nick.isNotEmpty()) {
										std::string_view mode_line = line.substr(std::min(line.find(' ', 2), line.size()));
										if (!mode_line.empty()) {
											mode_line.remove_prefix(1);
										}

										auto split_mode_line = jessilib::word_split_once_view(mode_line, ' ');
										std::string_view modes = split_mode_line.first;
										std::vector<std::string_view> mode_parameters = jessilib::word_split_view(split_mode_line.second, ' ');

										size_t word_index = 0;
										char sign = 0;
										for (auto mode : modes) {
											if (mode == '+' || mode == '-') {
												sign = mode;
											}
											else if (m_prefix_modes.find(mode) != std::string::npos) { // user prefix mode
												std::string_view mode_parameter;
												if (word_index < mode_parameters.size()) {
													mode_parameter = mode_parameters[word_index];
												}

												if (!mode_parameter.empty() && mode_parameter.front() == ':') {
													// Strip leading ':'
													// TODO: actually populate mode_parameter correctly with the full parameter string
													mode_parameter.remove_prefix(1);

													// erase modeParameters; all further parameters are a part of this one
													mode_parameters.clear();
												}

												if (!mode_parameter.empty()) {
													Jupiter::IRC::Client::Channel* channel = getChannel(channel_name);
													if (channel != nullptr) {
														if (sign == '+') {
															channel->addUserPrefix(mode_parameter,
																m_prefixes[m_prefix_modes.find(mode)]);
														}
														else {
															channel->delUserPrefix(mode_parameter,
																m_prefixes[m_prefix_modes.find(mode)]);
														}
													}
												}
												++word_index;
											}
											else if (m_modeA.find(mode) != std::string::npos) { // mode type A
												++word_index;
											}
											else if (m_modeB.find(mode) != std::string::npos) { // mode type B
												++word_index;
											}
											else if (m_modeC.find(mode) != std::string::npos
												&& sign == '+') { // mode type C (with parameter)
												++word_index;
											}
											// else; // mode type D
										}

										this->OnMode(channel_name, nick, mode_line);
										for (auto& plugin: Jupiter::plugins) {
											plugin->OnMode(this, channel_name, nick, mode_line);
										}
									}
								}
							}
						}
							// else if ACCOUNT
							// else if CHGHOST
						else if (numeric == Reply::NAMREPLY) { // Some names.
							std::string_view channel_name = getLineToken(4);
							std::string_view names = jessilib::split_once_view(line.substr(1), ':').second;

							Channel* channel = getChannel(channel_name);
							if (channel != nullptr) {
								if (channel->m_adding_names == false) {
									Client::delChannel(channel_name);
									Client::addChannel(channel_name);
									channel = Jupiter::IRC::Client::getChannel(channel_name);
									channel->m_adding_names = true;
								}

								// addNamesToChannel can be cut/pasted here
								Client::addNamesToChannel(*channel, names);
							}
						}
						else if (numeric == Reply::ENDOFNAMES) { // We're done here.
							std::string_view channel_name = getLineToken(3);
							Channel* channel = getChannel(channel_name);
							if (channel != nullptr) {
								channel->m_adding_names = false;
							}
						}
						break;
					}
					}
				}
			}
			else
			{
				if (w1 == "PING"sv)
				{
					m_socket->send(Jupiter::StringS::Format("PONG %.*s" ENDL, first_split.second.size(), first_split.second.data()));
				}
				else if (w1 == "NICK"sv)
				{
					// TODO: How should we be handling whitespaces in first_split.second?
					if (!first_split.second.empty())
						m_nickname = first_split.second;
				}
				else if (w1 == "ERROR"sv)
				{
					std::string_view reason = jessilib::split_once_view(line, ':').second;
					this->OnError(reason);
					for (auto& plugin : Jupiter::plugins) {
						plugin->OnError(this, reason);
					}
					Jupiter::IRC::Client::disconnect();
				}
				else if (w1 == "AUTHENTICATE"sv)
				{
					if (!m_sasl_password.empty())
					{
						Jupiter::StringS auth_str = m_nickname + '\0' + m_sasl_account + '\0' + m_sasl_password;

						char *enc = Jupiter::base64encode(auth_str.data(), auth_str.size());
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
				for (auto& plugin : Jupiter::plugins) {
					plugin->OnNumeric(this, numeric, line);
				}
			}
		}
		this->OnRaw(line);
		for (auto& plugin : Jupiter::plugins) {
			plugin->OnRaw(this, line);
		}
	}

	return 0;
}

bool Jupiter::IRC::Client::connect()
{
	std::string_view clientAddress = Jupiter::IRC::Client::readConfigValue("ClientAddress"_jrs);
	if (m_socket->connect(m_server_hostname.c_str(), m_server_port, clientAddress.empty() ? nullptr : static_cast<std::string>(clientAddress).c_str(), (unsigned short)Jupiter::IRC::Client::readConfigLong("ClientPort"_jrs)) == false)
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
			if (!m_ssl_certificate.empty())
				t->setCertificate(m_ssl_certificate, m_ssl_key);

			m_socket.reset(t);
		}
		else
		{
			Jupiter::TCPSocket *t = new Jupiter::TCPSocket(std::move(*m_socket));
			m_socket.reset(t);
		}
	}
	for (auto& plugin : Jupiter::plugins) {
		plugin->OnDisconnect(this);
	}
}

void Jupiter::IRC::Client::disconnect(const Jupiter::ReadableString &message, bool stayDead)
{
	m_socket->send(Jupiter::StringS::Format("QUIT :%.*s" ENDL, message.size(), message.data()));
	Jupiter::IRC::Client::disconnect(stayDead);
}

void Jupiter::IRC::Client::reconnect()
{
	if (m_connection_status != 0) Jupiter::IRC::Client::disconnect();
	m_reconnect_attempts++;
	bool successConnect = Jupiter::IRC::Client::connect();
	this->OnReconnectAttempt(successConnect);
	for (auto& plugin : Jupiter::plugins) {
		plugin->OnReconnectAttempt(this, successConnect);
	}
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
		using namespace std::literals;
		auto tokens = jessilib::split_view(m_socket->getBuffer(), "\r\n"sv);
		if (tokens.size() != 0) {
			if (tokens[0].size() > 0) {
				// Ensure there's not a token getting split over separate buffers
				std::string_view token_0 = tokens[0];
				if (m_last_line.size() > 0) {
					if (tokens[0][0] == '\n' && m_last_line[m_last_line.size() - 1] == '\r') {
						m_last_line.pop_back();
						Jupiter::IRC::Client::process_line(m_last_line);
						m_last_line.erase();

						token_0.remove_prefix(1);
					}
				}

				m_last_line += token_0;
			}

			if (tokens.size() != 1) {
				Jupiter::IRC::Client::process_line(m_last_line);
				m_last_line = tokens[tokens.size() - 1];

				for (size_t index = 1; index != tokens.size() - 1; ++index) {
					if (Jupiter::IRC::Client::process_line(tokens[index]) != 0) {
						return handle_error(1);
					}
				}
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

std::string_view Jupiter::IRC::Client::readConfigValue(const Jupiter::ReadableString &key, std::string_view defaultValue) const {
	if (m_primary_section != nullptr) {
		std::string_view val = m_primary_section->get(key);
		if (!val.empty()) {
			return val;
		}
	}

	if (m_secondary_section != nullptr) {
		return m_secondary_section->get(key, defaultValue);
	}

	return defaultValue;
}

bool Jupiter::IRC::Client::readConfigBool(const Jupiter::ReadableString &key, bool defaultValue) const {
	if (m_primary_section != nullptr) {
		std::string_view val = m_primary_section->get(key);

		if (!val.empty()) {
			return Jupiter::from_string<bool>(val);
		}
	}

	if (m_secondary_section != nullptr) {
		return m_secondary_section->get<bool>(key, defaultValue);
	}

	return defaultValue;
}

int Jupiter::IRC::Client::readConfigInt(const Jupiter::ReadableString &key, int defaultValue) const {
	if (m_primary_section != nullptr) {
		std::string_view val = m_primary_section->get(key);

		if (!val.empty()) {
			return Jupiter::from_string<int>(val);
		}
	}

	if (m_secondary_section != nullptr) {
		return m_secondary_section->get<int>(key, defaultValue);
	}

	return defaultValue;
}

long Jupiter::IRC::Client::readConfigLong(const Jupiter::ReadableString &key, long defaultValue) const {
	if (m_primary_section != nullptr) {
		std::string_view val = m_primary_section->get(key);

		if (!val.empty()) {
			return Jupiter::from_string<long>(val);
		}
	}

	if (m_secondary_section != nullptr)
		return m_secondary_section->get<long>(key, defaultValue);

	return defaultValue;
}

double Jupiter::IRC::Client::readConfigDouble(const Jupiter::ReadableString &key, double defaultValue) const {
	if (m_primary_section != nullptr) {
		std::string_view val = m_primary_section->get(key);

		if (!val.empty()) {
			return Jupiter::from_string<double>(val);
		}
	}

	if (m_secondary_section != nullptr) {
		return m_secondary_section->get<double>(key, defaultValue);
	}

	return defaultValue;
}

void Jupiter::IRC::Client::writeToLogs(const Jupiter::ReadableString &message) {
	if (m_log_file != nullptr) {
		fwrite(message.data(), sizeof(char), message.size(), m_log_file);
		fputs("\r\n", m_log_file);
		fflush(m_log_file);
	}
}

/**
* @brief IRC Client Private Function Implementations
*/

void Jupiter::IRC::Client::delChannel(std::string_view in_channel) {
	m_channels.erase(Jupiter::ReferenceString{in_channel});
}

std::shared_ptr<Jupiter::IRC::Client::User> Jupiter::IRC::Client::findUser(const Jupiter::ReadableString &in_nickname) const {
	return getUser(in_nickname);
}

std::shared_ptr<Jupiter::IRC::Client::User> Jupiter::IRC::Client::findUserOrAdd(std::string_view in_name) {
	// in_name: Nickname!Username@Hostname
	// Assume: Nickname can contain anything but !
	// Assume: Username can contain anything but @
	// Assume: Hostname can contain anything
	auto name_split = jessilib::split_once(in_name, '!');

	auto result = getUser(name_split.first);
	if (result != nullptr) {
		return result;
	}

	auto username_hostname = jessilib::split_once(name_split.second, '@');

	auto user = std::make_shared<User>();
	user->m_nickname = name_split.first;
	user->m_username = username_hostname.first;
	user->m_hostname = username_hostname.second;
	return m_users.emplace(user->m_nickname, user).first->second;
}

void Jupiter::IRC::Client::addNamesToChannel(Channel &in_channel, std::string_view in_names) {
	size_t offset;
	std::vector<std::string_view> names = jessilib::split_view(in_names, ' ');
	for (auto name : names) {
		if (name.empty()) {
			// Good candidate for a warning log
			continue;
		}

		offset = name.find_first_not_of(m_prefixes);
		if (offset == std::string_view::npos) {
			// The user's name is nothing but prefixes!? Good candidate for an error log
			continue;
		}

		// Add the user
		auto user = Client::findUserOrAdd(name.substr(offset));
		in_channel.addUser(user);

		// Add any prefixes we received to the user
		while (offset > 0) {
			--offset;
			in_channel.addUserPrefix(user->getNickname(), name[offset]);
		}
	}
}

void Jupiter::IRC::Client::addChannel(std::string_view in_channel) {
	Jupiter::ReferenceString channel_name = in_channel;
	m_channels.emplace(channel_name, Channel(channel_name, this));
}

bool Jupiter::IRC::Client::startCAP() {
	m_connection_status = 2;
	return m_socket->send("CAP LS"sv ENDL) > 0;
}

bool Jupiter::IRC::Client::registerClient() {
	bool result = true;
	const char *localHostname = Jupiter::Socket::getLocalHostname();
	Jupiter::StringS messageToSend;

	messageToSend.format("USER %.*s %s %.*s :%.*s" ENDL, m_nickname.size(), m_nickname.data(), localHostname, m_server_hostname.size(), m_server_hostname.c_str(), m_realname.size(), m_realname.data());
	
	if (m_socket->send(messageToSend) <= 0)
		result = false;

	messageToSend.format("NICK %.*s" ENDL, m_nickname.size(), m_nickname.data());

	if (m_socket->send(messageToSend) <= 0)
		result = false;

	m_connection_status = 3;
	return result;
}

/**
* User Implementation
*/

std::string_view Jupiter::IRC::Client::User::getNickname() const {
	return m_nickname;
}

std::string_view Jupiter::IRC::Client::User::getUsername() const {
	return m_username;
}

std::string_view Jupiter::IRC::Client::User::getHostname() const {
	return m_hostname;
}

size_t Jupiter::IRC::Client::User::getChannelCount() const {
	return m_channel_count;
}

/**
* Channel Implementation
*/

Jupiter::IRC::Client::Channel::Channel(const Jupiter::ReadableString &in_name, Jupiter::IRC::Client *in_parent) {
	auto to_lower = [&in_name]() {
		Jupiter::String result(in_name.size());
		const char *itr = in_name.data();
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

	auto read_type = [&name](Jupiter::Config &in_config, int default_type) {
		return in_config["Channels"_jrs][name].get<int>("Type"_jrs, default_type);
	};

	if (m_parent->getSecondaryConfigSection() != nullptr)
		m_type = read_type(*m_parent->getSecondaryConfigSection(), m_type);

	if (m_parent->getPrimaryConfigSection() != nullptr)
		m_type = read_type(*m_parent->getPrimaryConfigSection(), m_type);
}

std::shared_ptr<Jupiter::IRC::Client::Channel::User> Jupiter::IRC::Client::Channel::addUser(std::shared_ptr<Client::User> user) {
	auto channel_user = std::make_shared<Channel::User>();
	channel_user->m_user = user;

	++user->m_channel_count;

	m_users[Jupiter::ReferenceString{channel_user->getNickname()}] = channel_user;
	return channel_user;
}

std::shared_ptr<Jupiter::IRC::Client::Channel::User> Jupiter::IRC::Client::Channel::addUser(std::shared_ptr<Client::User> user, const char prefix) {
	auto channel_user = std::make_shared<Channel::User>();
	channel_user->m_user = user;
	channel_user->m_prefixes = prefix;

	++user->m_channel_count;

	m_users[Jupiter::ReferenceString{channel_user->getNickname()}] = channel_user;
	return channel_user;
}

void Jupiter::IRC::Client::Channel::delUser(std::string_view in_nickname) {
	m_users.erase(Jupiter::ReferenceString{in_nickname});
}

void Jupiter::IRC::Client::Channel::addUserPrefix(std::string_view in_nickname, char prefix) {
	auto user = getUser(in_nickname);

	if (user != nullptr && user->m_prefixes.find(prefix) == std::string_view::npos)
		user->m_prefixes += prefix;
}

void Jupiter::IRC::Client::Channel::delUserPrefix(std::string_view in_nickname, char prefix) {
	auto user = getUser(in_nickname);

	if (user != nullptr)
		user->m_prefixes.remove(prefix);
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::getName() const {
	return m_name;
}

std::shared_ptr<Jupiter::IRC::Client::Channel::User> Jupiter::IRC::Client::Channel::getUser(std::string_view in_nickname) const {
	auto user = m_users.find(Jupiter::ReferenceString{in_nickname});
	if (user != m_users.end()) {
		return user->second;
	}

	return nullptr;
}

static_assert(Jupiter::INVALID_INDEX == std::string_view::npos);

char Jupiter::IRC::Client::Channel::getUserPrefix(const Channel::User& in_user) const {
	std::string_view prefixes = m_parent->getPrefixes();
	for (auto prefix : prefixes) {
		if (in_user.m_prefixes.find(prefix) != std::string_view::npos) {
			return prefix;
		}
	}

	return 0;
}

char Jupiter::IRC::Client::Channel::getUserPrefix(std::string_view in_nickname) const {
	auto user = getUser(in_nickname);

	if (user != nullptr)
		return this->getUserPrefix(*user);

	return 0;
}

const Jupiter::IRC::Client::Channel::UserTableType &Jupiter::IRC::Client::Channel::getUsers() const {
	return m_users;
}

size_t Jupiter::IRC::Client::Channel::getUserCount() const {
	return m_users.size();
}

int Jupiter::IRC::Client::Channel::getType() const {
	return m_type;
}

void Jupiter::IRC::Client::Channel::setType(int in_type) {
	m_type = in_type;
}

/**
* Channel User Implementation
*/

Jupiter::IRC::Client::Channel::User::~User() {
	if (m_user != nullptr)
		--m_user->m_channel_count;
}

Jupiter::IRC::Client::User *Jupiter::IRC::Client::Channel::User::getUser() const {
	return m_user.get();
}

const Jupiter::ReadableString &Jupiter::IRC::Client::Channel::User::getPrefixes() const {
	return m_prefixes;
}

std::string_view Jupiter::IRC::Client::Channel::User::getNickname() const {
	return m_user->getNickname();
}

std::string_view Jupiter::IRC::Client::Channel::User::getUsername() const {
	return m_user->getUsername();
}

std::string_view Jupiter::IRC::Client::Channel::User::getHostname() const {
	return m_user->getHostname();
}

size_t Jupiter::IRC::Client::Channel::User::getChannelCount() const {
	return m_user->getChannelCount();
}