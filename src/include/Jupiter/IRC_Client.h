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

#if !defined _IRC_CLIENT_H_HEADER
#define _IRC_CLIENT_H_HEADER

/**
 * @file IRC Client.h
 * @brief Provides connectivity to IRC servers.
 */

#include <cstdlib>
#include <cstdio>
#include <utility>
#include "Jupiter.h"
#include "Thinker.h"
#include "IRC.h"
#include "Reference_String.h"
#include "Config.h"
#include "Socket.h"

/** DLL Linkage Nagging */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace Jupiter
{
	namespace IRC
	{
		/**
		* @brief Provides connectivity to IRC servers.
		*/
		class JUPITER_API Client : public Jupiter::Thinker
		{
		protected:

			/**
			* @brief This is called when a connection has been successfully established.
			* The current implementation calls this after the MOTD.
			*/
			virtual void OnConnect();

			/**
			* @brief This is called when at the end of disconnect().
			* This is called upon any connection failure or error.
			*/
			virtual void OnDisconnect();

			/**
			* @brief This is called when at the end of reconnect().
			*
			* @param in_success Used to determine if a connection was successfully established.
			* @see connect().
			*/
			virtual void OnReconnectAttempt(bool in_success);

			/**
			* @brief This is called after a message has been normally processed.
			* This always happens last. All other hooks will be called before this.
			*
			* @param in_message The raw message.
			*/
			virtual void OnRaw(std::string_view in_message);

			/**
			* @brief This is called after an IRC numeric has been processed.
			*
			* @param in_numerouic The numeroic of the message
			* @param in_message The raw message.
			*/
			virtual void OnNumeric(long int in_numeric, std::string_view in_message);

			/**
			* @brief This is called when an ERROR is received.
			* This indicates a connection termination, and thus, disconnect() is called immediately after this.
			*
			* @param message Message sent by the server.
			*/
			virtual void OnError(std::string_view in_message);

			/**
			* @brief This is called when a chat message is received.
			*
			* @param channel String containing the destination of the message.
			* @param nick String containing the nickname of the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnChat(std::string_view in_channel, std::string_view in_nickname, std::string_view in_message);

			/**
			* @brief This is called when a notice is received.
			*
			* @param chan String containing the destination of the message.
			* @param nick String containing the nickname of the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnNotice(std::string_view in_channel, std::string_view in_sender, std::string_view in_message);

			/**
			* @brief This is called when a server notice is received.
			*
			* @param chan String containing the destination of the message.
			* @param nick String containing the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnServerNotice(std::string_view in_channel, std::string_view in_sender, std::string_view in_message);

			/**
			* @brief This is called when a CTCP message is received.
			*
			* @param channel String containing the destination of the message.
			* @param nick String containing the nickname of the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnCTCP(std::string_view in_channel, std::string_view in_nickname, std::string_view in_command, std::string_view in_message);

			/**
			* @brief This is called when an action message is received.
			*
			* @param chan String containing the destination of the message.
			* @param nick String containing the nickname of the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnAction(std::string_view in_channel, std::string_view in_nickname, std::string_view in_message);

			/**
			* @brief This is called when an invite is received.
			*
			* @param chan String containing the destination of the message.
			* @param inviter String containing the nickname of the inviter.
			* @param invited String containing the nickname of the user invited.
			*/
			virtual void OnInvite(std::string_view in_channel, std::string_view in_inviter, std::string_view in_invited);

			/**
			* @brief This is called when a chat message is received.
			*
			* @param chan String containing the destination of the message.
			* @param nick String containing the nickname of the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnJoin(std::string_view in_channel, std::string_view in_nickname);

			/**
			* @brief This is called when a user parts a channel.
			*
			* @param chan String containing the channel parted from.
			* @param nick String containing the nickname of the user.
			* @param reason String containing the reason for parting, or nullptr if none is specified.
			*/
			virtual void OnPart(std::string_view in_channel, std::string_view in_nickname, std::string_view in_reason);

			/**
			* @brief This is called when a user changes their nickname.
			*
			* @param oldnick String containing the old nickname of the user.
			* @param newnick String containing the new nickname of the user.
			*/
			virtual void OnNick(std::string_view in_old_nick, std::string_view in_new_nick);

			/**
			* @brief This is called when a user is kicked from a channel.
			*
			* @param chan String containing the channel kicked from.
			* @param kicker String containing the nickname of the kicker.
			* @param kicked String containing the nickname of the user kicked.
			* @param reason String containing the reason for the kick, or nullptr if none is specified.
			*/
			virtual void OnKick(std::string_view in_channel, std::string_view in_kicker, std::string_view in_kicked, std::string_view in_reason);

			/**
			* @brief This is called when a user quits the server.
			*
			* @param nick String containing the nickname of the user.
			* @param message String containing the reason for quiting.
			*/
			virtual void OnQuit(std::string_view in_nickname, std::string_view in_message);

			/**
			* @brief This is called when a channel mode is changed.
			*
			* @param chan String containing the affected channel.
			* @param nick String containing the nickname of the user.
			* @param modeString String containing the modes changed.
			*/
			virtual void OnMode(std::string_view in_channel, std::string_view in_nickname, std::string_view in_mode_string);
		public:
			class Channel;

			/**
			* @brief Stores data about users.
			*/
			class JUPITER_API User
			{
				friend class Jupiter::IRC::Client;
				friend class Jupiter::IRC::Client::Channel;
			public:

				/**
				* @brief Fetches the user's nickname.
				*
				* @return String containing the user's nickname.
				*/
				std::string_view getNickname() const;

				/**
				* @brief Fetches the user's username.
				*
				* @return String containing the user's username.
				*/
				std::string_view getUsername() const;

				/**
				* @brief Fetches the user's hostname.
				*
				* @return String containing the user's hostname.
				*/
				std::string_view getHostname() const;

				/**
				* @brief Returns the number of channels the user shares with the local client.
				*
				* @return Number of channels.
				*/
				size_t getChannelCount() const;

				/** Private members */
			private:
				unsigned int m_channel_count = 0;
				std::string m_nickname;
				Jupiter::StringS m_username;
				Jupiter::StringS m_hostname;
			};

			/**
			* @brief Provides interfacing with channels and stores channel data.
			*/
			class JUPITER_API Channel
			{
				friend class Jupiter::IRC::Client;
			public:

				/**
				* @brief Stores data about users.
				*/
				class JUPITER_API User
				{
					friend class Jupiter::IRC::Client::Channel;
				public:

					/**
					* @brief Returns the local user's global user object.
					*
					* @return Local user's global user object.
					*/
					Jupiter::IRC::Client::User *getUser() const;

					/**
					* @brief Returns the user's string of channel prefixes.
					*
					* @return String containing the user's channel prefixes.
					*/
					const Jupiter::ReadableString &getPrefixes() const;

					/**
					* @brief Fetches the user's nickname.
					*
					* @return String containing the user's nickname.
					*/
					std::string_view getNickname() const;

					/**
					* @brief Fetches the user's username.
					*
					* @return String containing the user's username.
					*/
					std::string_view getUsername() const;

					/**
					* @brief Fetches the user's hostname.
					*
					* @return String containing the user's hostname.
					*/
					std::string_view getHostname() const;

					/**
					* @brief Returns the number of channels the user shares with the local client.
					*
					* @return Number of channels.
					*/
					size_t getChannelCount() const;

					~User();

				/** Private members */
				private:
					std::shared_ptr<Jupiter::IRC::Client::User> m_user;
					Jupiter::StringS m_prefixes;
				};

				using UserTableType = std::unordered_map<Jupiter::StringS, std::shared_ptr<Channel::User>, default_hash_function>;

				/**
				* @brief Returns the name of the channel.
				*
				* @return String containing the name of the channel.
				*/
				const Jupiter::ReadableString &getName() const;

				/**
				* @brief Searches for a user based on their nickname.
				*
				* @param nickname String containing the nickname of the user to find.
				* @return A user if a match is found, nullptr otherwise.
				*/
				std::shared_ptr<Channel::User> getUser(std::string_view in_nickname) const;

				/**
				* @brief Adds a user to the channel
				*
				* @param nickname Nickname of the user.
				* @return Index of the new user.
				*/
				std::shared_ptr<Channel::User> addUser(std::shared_ptr<Client::User> in_user);

				/**
				* @brief Adds a user to the channel
				*
				* @param nickname Nickname of the user.
				* @param prefix The user's prefix.
				* @return Index of the new user.
				*/
				std::shared_ptr<Channel::User> addUser(std::shared_ptr<Client::User> in_user, const char in_prefix);

				/**
				* @brief Removes a user from the channel.
				*
				* @param nickname String containing the nickname of the user.
				*/
				void delUser(std::string_view in_nickname);

				/**
				* @brief Adds a prefix to a user.
				*
				* @param user String containing the nickname of the user.
				* @param prefix Prefix to add to the user.
				*/
				void addUserPrefix(std::string_view in_nickname, char in_prefix);

				/**
				* @brief Removes a prefix from a user.
				*
				* @param user String containing the nickname of a user.
				* @param prefix Prefix to remove from the user.
				*/
				void delUserPrefix(std::string_view in_nickname, char in_prefix);

				/**
				* @brief Returns a user's most significant prefix.
				*
				* @param user String containing the nickname of a user.
				* @return User's most significant prefix.
				*/
				char getUserPrefix(const Channel::User &in_user) const;
				char getUserPrefix(std::string_view in_nickname) const;

				/**
				* @brief Fetches the channel's user table
				*
				* @return Reference to the channel's user table
				*/
				const UserTableType &getUsers() const;

				/**
				* @brief Returns the number of users in this channel.
				*
				* @return Number of users.
				*/
				size_t getUserCount() const;

				/**
				* @brief Returns the type of this channel.
				*
				* @return Type of this channel.
				*/
				int getType() const;

				/**
				* @brief Sets the type of this channel.
				*
				* @param iType Type to set this channel to.
				*/
				void setType(int iType);

				Channel() = default;

				/**
				* @brief Constructor for Channel
				*
				* @param channelName String containing the name of a channel.
				* @param iFace Server in which this channel is located.
				*/
				Channel(const Jupiter::ReadableString &in_name, Client *in_parent);

			/** Private members */
			private:
				Jupiter::StringS m_name;
				Client *m_parent;
				int m_type;
				UserTableType m_users;

				bool m_adding_names;
			}; // Jupiter::IRC::Client::Channel class

			using ChannelTableType = std::unordered_map<Jupiter::StringS, Client::Channel, default_hash_function>;
			using UserTableType = std::unordered_map<Jupiter::StringS, std::shared_ptr<Client::User>, default_hash_function>;

			/**
			* @brief Returns the name of the primary config section this client reads from.
			*
			* @return String containing a config section's name.
			*/
			std::string_view getConfigSection() const;

			/**
			* @brief Fetches the primary config section
			*
			* @return The primary config section if it exists, nullptr otherwise.
			*/
			Jupiter::Config *getPrimaryConfigSection() const;

			/**
			* @brief Fetches the primary config section
			*
			* @return The primary config section if it exists, nullptr otherwise.
			*/
			Jupiter::Config *getSecondaryConfigSection() const;

			/**
			* @brief Sets the primary config section
			* Note: This is useful when the higher config file is reloaded
			*
			* @param in_primary_section Primary config section to begin using
			*/
			virtual void setPrimaryConfigSection(Jupiter::Config *in_primary_section);

			/**
			* @brief Sets the secondary config section
			* Note: This is useful when the higher config file is reloaded
			*
			* @param in_secondary_section Secondary config section to begin using
			*/
			virtual void setSecondaryConfigSection(Jupiter::Config *in_secondary_section);

			/**
			* @brief Returns the name of the file this logs to.
			*
			* @return String containing a log file's name.
			*/
			const std::string &getLogFile() const;

			/**
			* @brief Returns the nickname prefixes supported by the connected server.
			*
			* @return String containing nickname prefixes.
			*/
			std::string_view getPrefixes() const;

			/**
			* @brief Returns mode symbols for nickname prefixes supported by the connected server.
			*
			* @return String containing mode symbols for nickname prefixes.
			*/
			std::string_view getPrefixModes() const;

			/**
			* @brief Returns the client's current nickname.
			*
			* @return String containing a nickame.
			*/
			std::string_view getNickname() const;

			/**
			* @brief Returns the client's real name.
			*
			* @return String containing a name.
			*/
			std::string_view getRealname() const;

			/**
			* @brief Returns the server's name
			*
			* @return String containing the server's name.
			*/
			const Jupiter::ReadableString &getServerName() const;

			/**
			* @brief Returns the server's hostname.
			* Note: This is the same as the config's value.
			*
			* @return String containing a hostname.
			*/
			const std::string &getServerHostname() const;

			/**
			* @brief Returns the server's port.
			* Note: This is the same as the config's value.
			*
			* @return Port used to connect.
			*/
			unsigned short getServerPort() const;

			/**
			* @brief Returns the time delay between reconnect attempts.
			*
			* @return Time delay between reconnect attemps.
			*/
			time_t getReconnectDelay() const;

			/**
			* @brief Returns the time scheduled to make a reconenct attempt.
			* This is the sum of the time of disconnection and the reconnection time delay.
			*
			* @return Time scheduled.
			*/
			time_t getReconnectTime() const;

			/**
			* @brief Returns the number of consecutive reconnect attempts executed so far.
			*
			* @return Number of reconnects attempted.
			*/
			int getReconnectAttempts() const;

			/**
			* @brief Returns the maximum number of consecutive reconnect attempts to make before failing.
			*
			* @return Maximum number of reconnect attempts.
			*/
			int getMaxReconnectAttempts() const;

			/**
			* @brief Returns the default channel type.
			*
			* @return Default channel type.
			*/
			int getDefaultChanType() const;

			/**
			* @brief Returns if this prints to stdout.
			*
			* @return True if it prints, false otherwise.
			*/
			FILE *getPrintOutput() const;

			/**
			* @brief Sets where to print output.
			*
			* @param outf FILE object to output to.
			* Note: outf must be either a valid and open FILE pointer, or nullptr to disable output.
			*/
			void setPrintOutput(FILE *outf);

			/**
			* @brief Fetches the channel table
			*
			* @return Channel table
			*/
			const UserTableType &getUsers() const;

			/**
			* @brief Fetches the size of the user list.
			*
			* @return Size of the user list.
			*/
			size_t getUserCount() const;

			/**
			* @brief Fetches a user from the user list.
			*
			* @param nickname String containing the nickname of the user to fetch.
			* @return A User if a match is found, nullptr otherwise.
			*/
			std::shared_ptr<User> getUser(std::string_view in_nickname) const;

			/**
			* @brief Fetches the channel table
			*
			* @return Channel table
			*/
			const ChannelTableType &getChannels() const;

			/**
			* @brief Returns the number of channels.
			*
			* @return Number of channels.
			*/
			size_t getChannelCount() const;

			/**
			* @brief Returns a channel.
			*
			* @param chanName String containing the name of a channel.
			* @return A channel with the specified name if it exists, nullptr otherwise.
			*/
			Channel *getChannel(std::string_view in_channel) const;

			/**
			* @brief Sends a join request.
			*
			* @param channel Channel to join.
			*/
			void joinChannel(std::string_view in_channel);

			/**
			* @brief Sends a join request with a password.
			*
			* @param channel Channel to join.
			* @param password Password to use.
			*/
			void joinChannel(std::string_view in_channel, std::string_view in_password);

			/**
			* @brief Parts a channel.
			*
			* @param channel Channel to part.
			*/
			void partChannel(std::string_view in_channel);

			/**
			* @brief Parts a channel.
			*
			* @param channel Channel to part.
			* @param message Reason for parting.
			*/
			void partChannel(std::string_view in_channel, std::string_view in_message);

			/**
			* @brief Gets the access level of a user.
			*
			* @param chan String containing a channel name to get the access level from.
			* @param nick String containing the nickname of the user.
			* @return Access level of the user.
			*/
			int getAccessLevel(const Channel &in_channel, std::string_view in_nickname) const;
			int getAccessLevel(std::string_view in_channel, std::string_view in_nickname) const;

			/**
			* @brief Sends a message.
			*
			* @param dest String containing the destination of the message (nickname or channel).
			* @param message String containing the message to send.
			*/
			void sendMessage(std::string_view in_destination, std::string_view in_message);

			/**
			* @brief Sends a notice.
			*
			* @param dest String containing the destination of the message (nickname or channel).
			* @param message String containing the message to send.
			*/
			void sendNotice(std::string_view in_destination, std::string_view in_message);

			/**
			* @brief Sends a message to all channels of a given type.
			*
			* @param type Type of channel to messasge.
			* @param message String containing the message to send.
			* @return Number of messages sent.
			*/
			size_t messageChannels(int type, std::string_view in_message);

			/**
			* @brief Sends a message to all channels with a type of at least 0.
			*
			* @param message String containing the message to send.
			* @return Number of messages sent.
			*/
			size_t messageChannels(const Jupiter::ReadableString &in_message);

			/**
			* @brief Returns if the client will automatically reconnect upon failure.
			*
			* @return True if the server will automatically reconnect, false otherwise.
			*/
			bool isAutoReconnect() const;

			/**
			* @brief Sets how many times the client should attempt to automatically reconnect.
			*
			* @param autoReconnect Maximum number of reconnect attempts. A negative value indicates infinity.
			*/
			void setAutoReconnect(int in_auto_reconnect);

			/**
			* @brief Sends data to the server.
			* Endlines are automatically added.
			*
			* @param rawMessage String containing the data to send.
			*/
			void send(const Jupiter::ReadableString &in_message);

			/**
			* @brief Processes an input line of IRC protocol data.
			*
			* @return 0 upon success, an error code otherwise.
			*/
			int process_line(std::string_view in_line);

			/**
			* @brief Returns a key's value.
			* This reads from the client's config section first, then default if it doesn't exist.
			*
			* @param key String containing the key name.
			* @param in_default_value Optional parameter specifying the default value to return if none is found.
			* @return String containing the key value if it exists, in_default_value otherwise.
			*/
			std::string_view readConfigValue(const Jupiter::ReadableString &key, std::string_view in_default_value = std::string_view{}) const;

			/**
			* @brief Returns a key's value as a boolean.
			* This reads from the client's config section first, then default if it doesn't exist.
			*
			* @param key String containing the key name.
			* @return Boolean value of the key value if it exists, in_default_value otherwise.
			*/
			bool readConfigBool(const Jupiter::ReadableString &key, bool in_default_value = false) const;

			/**
			* @brief Returns a key's value as an integer.
			* This reads from the client's config section first, then default if it doesn't exist.
			*
			* @param key String containing the key name.
			* @return Integer value of the key value if it exists, in_default_value otherwise.
			*/
			int readConfigInt(const Jupiter::ReadableString &key, int in_default_value = 0) const;

			/**
			* @brief Returns a key's value as a long integer.
			* This reads from the client's config section first, then default if it doesn't exist.
			*
			* @param key String containing the key name.
			* @return Long integer value of the key value if it exists, in_default_value otherwise.
			*/
			long readConfigLong(const Jupiter::ReadableString &key, long in_default_value = 0) const;

			/**
			* @brief Returns a key's value as a double.
			* This reads from the client's config section first, then default if it doesn't exist.
			*
			* @param key String containing the key name.
			* @return Double value of the key value if it exists, in_default_value otherwise.
			*/
			double readConfigDouble(const Jupiter::ReadableString &key, double in_default_value = 0) const;

			/**
			* @brief Writes to the server's log file.
			*
			* @param message String containing the text to write to the file.
			*/
			void writeToLogs(const Jupiter::ReadableString &in_message);

			/**
			* @brief Connects the client to its server.
			* Note: This should not be called unless it is not already connected to a server.
			* Also, a successful socket connection does not mean the server accepted the connection.
			*
			* @return True upon successful socket connection, false otherwise.
			*/
			bool connect();

			/**
			* @brief Completely disconnects the client from a server.
			* Note: This does not send any type of QUIT message.
			*/
			void disconnect(bool in_stay_dead = false);

			/**
			* @brief Completely disconnects the client from a server.
			*
			* @param message String containing the QUIT message to send prior to disconnecting.
			*/
			void disconnect(const Jupiter::ReadableString &in_message, bool in_stay_dead = false);

			/**
			* @brief Calls disconnect() if the client has not already, then calls connect().
			* Note: This will increment the current reconnect attempts by 1.
			*/
			void reconnect();

			/**
			* @brief Method to be looped to sustain the IRC connection.
			* Note: This client should be deleted if this returns anything other than 0.
			*
			* @return 0 if the client should still exist, error code otherwise.
			*/
			virtual int think() override;

			/**
			* @brief Constructor for a client.
			*
			* @param in_primary_section Config section to search first for a configuration option
			* @param in_secondary_section Config section to search second for a configuration, before using a pre-defined default value
			*/
			Client(Jupiter::Config *in_primary_section, Jupiter::Config *in_secondary_section);

			/**
			* @brief Destructor for a client.
			*/
			virtual ~Client();

		/** Private members */
		private:
			std::unique_ptr<Jupiter::Socket> m_socket;
			uint16_t m_server_port;
			std::string m_server_hostname;

			bool m_ssl;
			std::string m_ssl_certificate;
			std::string m_ssl_key;

			std::string m_sasl_account;
			std::string m_sasl_password;

			int m_connection_status;
			Jupiter::StringS m_primary_section_name;
			Jupiter::Config *m_primary_section;
			Jupiter::Config *m_secondary_section;
			std::string m_log_file_name;
			std::string m_last_line;
			Jupiter::StringS m_server_name;
			std::string m_nickname;
			std::string m_realname;

			std::string m_prefix_modes = "ov";
			std::string m_prefixes = "@+";
			std::string m_chan_types = "#";
			std::string m_modeA = "b";
			std::string m_modeB = "k";
			std::string m_modeC = "l";
			std::string m_modeD = "psitnm";

			UserTableType m_users;
			ChannelTableType m_channels;

			bool m_join_on_kick;
			std::string m_auto_part_message;
			time_t m_reconnect_delay;
			time_t m_reconnect_time;
			int m_max_reconnect_attempts;
			int m_reconnect_attempts;
			FILE *m_output;
			FILE *m_log_file;
			int m_default_chan_type;
			bool m_dead = false;

			void delChannel(std::string_view in_channel);
			void addNamesToChannel(Channel &in_channel, std::string_view in_names);
			void addChannel(std::string_view in_channel);

			bool startCAP();
			bool registerClient();
			std::shared_ptr<User> findUser(const Jupiter::ReadableString &in_nickname) const;
			std::shared_ptr<User> findUserOrAdd(std::string_view in_nickname);
		}; // Jupiter::IRC::Client class

	} // Jupiter::IRC namespace

} // Jupiter namespace

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // _IRC_CLIENT_H_HEADER