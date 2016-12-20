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

#if !defined _IRC_CLIENT_H_HEADER
#define _IRC_CLIENT_H_HEADER

/**
 * @file IRC Client.h
 * @brief Provides connectivity to IRC servers.
 */

#include <cstdlib>
#include <cstdio>
#include "Jupiter.h"
#include "Thinker.h"
#include "IRC.h"
#include "Reference_String.h"
#include "Config.h"

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
			* @param successConnect Used to determine if a connection was successfully established.
			* @see connect().
			*/
			virtual void OnReconnectAttempt(bool successConnect);

			/**
			* @brief This is called after a message has been normally processed.
			* This always happens last. All other hooks will be called before this.
			*
			* @param raw The raw message.
			*/
			virtual void OnRaw(const Jupiter::ReadableString &raw);

			/**
			* @brief This is called after an IRC numeric has been processed.
			*
			* @param raw The raw message.
			*/
			virtual void OnNumeric(long int numeric, const Jupiter::ReadableString &raw);

			/**
			* @brief This is called when an ERROR is received.
			* This indicates a connection termination, and thus, disconnect() is called immediately after this.
			*
			* @param message Message sent by the server.
			*/
			virtual void OnError(const Jupiter::ReadableString &message);

			/**
			* @brief This is called when a chat message is received.
			*
			* @param channel String containing the destination of the message.
			* @param nick String containing the nickname of the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnChat(const Jupiter::ReadableString &channel, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &message);

			/**
			* @brief This is called when a notice is received.
			*
			* @param chan String containing the destination of the message.
			* @param nick String containing the nickname of the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnNotice(const Jupiter::ReadableString &chan, const Jupiter::ReadableString &sender, const Jupiter::ReadableString &message);

			/**
			* @brief This is called when a server notice is received.
			*
			* @param chan String containing the destination of the message.
			* @param nick String containing the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnServerNotice(const Jupiter::ReadableString &chan, const Jupiter::ReadableString &sender, const Jupiter::ReadableString &message);

			/**
			* @brief This is called when a CTCP message is received.
			*
			* @param channel String containing the destination of the message.
			* @param nick String containing the nickname of the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnCTCP(const Jupiter::ReadableString &channel, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &command, const Jupiter::ReadableString &message);

			/**
			* @brief This is called when an action message is received.
			*
			* @param chan String containing the destination of the message.
			* @param nick String containing the nickname of the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnAction(const Jupiter::ReadableString &chan, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &message);

			/**
			* @brief This is called when an invite is received.
			*
			* @param chan String containing the destination of the message.
			* @param inviter String containing the nickname of the inviter.
			* @param invited String containing the nickname of the user invited.
			*/
			virtual void OnInvite(const Jupiter::ReadableString &chan, const Jupiter::ReadableString &inviter, const Jupiter::ReadableString &invited);

			/**
			* @brief This is called when a chat message is received.
			*
			* @param chan String containing the destination of the message.
			* @param nick String containing the nickname of the sender.
			* @param message String containing the message sent.
			*/
			virtual void OnJoin(const Jupiter::ReadableString &chan, const Jupiter::ReadableString &nick);

			/**
			* @brief This is called when a user parts a channel.
			*
			* @param chan String containing the channel parted from.
			* @param nick String containing the nickname of the user.
			* @param reason String containing the reason for parting, or nullptr if none is specified.
			*/
			virtual void OnPart(const Jupiter::ReadableString &chan, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &reason);

			/**
			* @brief This is called when a user changes their nickname.
			*
			* @param oldnick String containing the old nickname of the user.
			* @param newnick String containing the new nickname of the user.
			*/
			virtual void OnNick(const Jupiter::ReadableString &oldnick, const Jupiter::ReadableString &newnick);

			/**
			* @brief This is called when a user is kicked from a channel.
			*
			* @param chan String containing the channel kicked from.
			* @param kicker String containing the nickname of the kicker.
			* @param kicked String containing the nickname of the user kicked.
			* @param reason String containing the reason for the kick, or nullptr if none is specified.
			*/
			virtual void OnKick(const Jupiter::ReadableString &chan, const Jupiter::ReadableString &kicker, const Jupiter::ReadableString &kicked, const Jupiter::ReadableString &reason);

			/**
			* @brief This is called when a user quits the server.
			*
			* @param nick String containing the nickname of the user.
			* @param message String containing the reason for quiting.
			*/
			virtual void OnQuit(const Jupiter::ReadableString &nick, const Jupiter::ReadableString &message);

			/**
			* @brief This is called when a channel mode is changed.
			*
			* @param chan String containing the affected channel.
			* @param nick String containing the nickname of the user.
			* @param modeString String containing the modes changed.
			*/
			virtual void OnMode(const Jupiter::ReadableString &chan, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &modeString);
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
				const Jupiter::ReadableString &getNickname() const;

				/**
				* @brief Fetches the user's username.
				*
				* @return String containing the user's username.
				*/
				const Jupiter::ReadableString &getUsername() const;

				/**
				* @brief Fetches the user's hostname.
				*
				* @return String containing the user's hostname.
				*/
				const Jupiter::ReadableString &getHostname() const;

				/**
				* @brief Returns the number of channels the user shares with the local client.
				*
				* @return Number of channels.
				*/
				size_t getChannelCount() const;

				User();
				~User();

				/** Private members */
			private:
				struct Data;
				Data *data_;
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
					const Jupiter::ReadableString &getNickname() const;

					/**
					* @brief Fetches the user's username.
					*
					* @return String containing the user's username.
					*/
					const Jupiter::ReadableString &getUsername() const;

					/**
					* @brief Fetches the user's hostname.
					*
					* @return String containing the user's hostname.
					*/
					const Jupiter::ReadableString &getHostname() const;

					/**
					* @brief Returns the number of channels the user shares with the local client.
					*
					* @return Number of channels.
					*/
					unsigned int getChannelCount() const;

					User();
					~User();

				/** Private members */
				private:
					struct Data;
					Data *data_;
				};

				/**
				* @brief Returns the name of the channel.
				*
				* @return String containing the name of the channel.
				*/
				const Jupiter::ReadableString &getName() const;

				/**
				* @brief Returns a user at an index.
				*
				* @param index The index of the user.
				* @return A user if within range, nullptr otherwise.
				*/
				Jupiter::IRC::Client::Channel::User *getUser(unsigned int index) const;

				/**
				* @brief Searches for a user based on their nickname.
				*
				* @param nickname String containing the nickname of the user to find.
				* @return A user if a match is found, nullptr otherwise.
				*/
				Jupiter::IRC::Client::Channel::User *getUser(const Jupiter::ReadableString &nickname) const;

				/**
				* @brief Adds a user to the channel
				*
				* @param nickname Nickname of the user.
				* @return Index of the new user.
				*/
				unsigned int addUser(Jupiter::IRC::Client::User *user);

				/**
				* @brief Adds a user to the channel
				*
				* @param nickname Nickname of the user.
				* @param prefix The user's prefix.
				* @return Index of the new user.
				*/
				unsigned int addUser(Jupiter::IRC::Client::User *user, const char prefix);

				/**
				* @brief Removes a user from the channel.
				*
				* @param nickname String containing the nickname of the user.
				*/
				void delUser(const Jupiter::ReadableString &nickname);

				/**
				* @brief Removes a user from the channel.
				*
				* @param index Index of a user.
				*/
				void delUser(size_t index);

				/**
				* @brief Adds a prefix to a user.
				*
				* @param index Index of a user.
				* @param prefix Prefix to add to the user.
				*/
				void addUserPrefix(size_t index, char prefix);

				/**
				* @brief Adds a prefix to a user.
				*
				* @param user String containing the nickname of the user.
				* @param prefix Prefix to add to the user.
				*/
				void addUserPrefix(const Jupiter::ReadableString &user, char prefix);

				/**
				* @brief Removes a prefix from a user.
				*
				* @param index Index of a user.
				* @param prefix Prefix to remove from the user.
				*/
				void delUserPrefix(unsigned int index, char prefix);

				/**
				* @brief Removes a prefix from a user.
				*
				* @param user String containing the nickname of a user.
				* @param prefix Prefix to remove from the user.
				*/
				void delUserPrefix(const Jupiter::ReadableString &user, char prefix);

				/**
				* @brief Returns the index of a user.
				*
				* @param user String containing the nickname of a user.
				* @return Index of a user if they're found, -1 otherwise.
				*/
				int getUserIndex(const Jupiter::ReadableString &user) const;

				/**
				* @brief Returns the index of a user.
				*
				* @param user String containing part of the nickname of a user.
				* @return Index of a user if they're found, -1 otherwise.
				*/
				int getUserIndexByPartName(const Jupiter::ReadableString &user) const;

				/**
				* @brief Returns a user's most significant prefix.
				*
				* @param index Index of a user.
				* @return User's most significant prefix.
				*/
				char getUserPrefix(unsigned int index) const;

				/**
				* @brief Returns a user's most significant prefix.
				*
				* @param user String containing the nickname of a user.
				* @return User's most significant prefix.
				*/
				char getUserPrefix(const Jupiter::ReadableString &user) const;

				/**
				* @brief Returns the number of users in this channel.
				*
				* @return Number of users.
				*/
				unsigned int getUserCount() const;

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

				/**
				* @brief Constructor for Channel
				*
				* @param channelName String containing the name of a channel.
				* @param iFace Server in which this channel is located.
				*/
				Channel(const Jupiter::ReadableString &channelName, Client *iFace);

				/**
				* @brief Destructor for Channel
				*/
				~Channel();

			/** Private members */
			private:
				struct Data;
				Data *data_;
			}; // Jupiter::IRC::Client::Channel class

			/**
			* @brief Returns the name of the primary config section this client reads from.
			*
			* @return String containing a config section's name.
			*/
			const Jupiter::ReadableString &getConfigSection() const;

			/**
			* @brief Fetches the primary config section
			*
			* @return The primary config section if it exists, nullptr otherwise.
			*/
			const Jupiter::Config *getPrimaryConfigSection() const;

			/**
			* @brief Fetches the primary config section
			*
			* @return The primary config section if it exists, nullptr otherwise.
			*/
			const Jupiter::Config *getSecondaryConfigSection() const;

			/**
			* @brief Sets the primary config section
			* Note: This is useful when the higher config file is reloaded
			*
			* @param in_primary_section Primary config section to begin using
			*/
			virtual void setPrimaryConfigSection(const Jupiter::Config *in_primary_section);

			/**
			* @brief Sets the secondary config section
			* Note: This is useful when the higher config file is reloaded
			*
			* @param in_secondary_section Secondary config section to begin using
			*/
			virtual void setSecondaryConfigSection(const Jupiter::Config *in_secondary_section);

			/**
			* @brief Returns the name of the file this logs to.
			*
			* @return String containing a log file's name.
			*/
			const Jupiter::ReadableString &getLogFile() const;

			/**
			* @brief Returns the nickname prefixes supported by the connected server.
			*
			* @return String containing nickname prefixes.
			*/
			const Jupiter::ReadableString &getPrefixes() const;

			/**
			* @brief Returns mode symbols for nickname prefixes supported by the connected server.
			*
			* @return String containing mode symbols for nickname prefixes.
			*/
			const Jupiter::ReadableString &getPrefixModes() const;

			/**
			* @brief Returns the client's current nickname.
			*
			* @return String containing a nickame.
			*/
			const Jupiter::ReadableString &getNickname() const;

			/**
			* @brief Returns the client's real name.
			*
			* @return String containing a name.
			*/
			const Jupiter::ReadableString &getRealname() const;

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
			const Jupiter::ReadableString &getServerHostname() const;

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
			* @brief Fetches a user from the user list.
			*
			* @param index Index of the user to fetch.
			* @return User located at the specified index.
			*/
			Jupiter::IRC::Client::User *getUser(unsigned int index) const;

			/**
			* @brief Fetches a user from the user list.
			*
			* @param nickname String containing the nickname of the user to fetch.
			* @return A User if a match is found, nullptr otherwise.
			*/
			Jupiter::IRC::Client::User *getUser(const Jupiter::ReadableString &nickname) const;

			/**
			* @brief Fetches a user's index from the user list.
			*
			* @param nickname String containing the nickname of the user to fetch.
			* @return The index of a user if a match is found, -1 otherwise.
			*/
			int getUserIndex(const Jupiter::ReadableString &nickname) const;

			/**
			* @brief Fetches a user's index from the user list.
			*
			* @param user Pointer to a user's data.
			* @return The index of a user if a match is found, -1 otherwise.
			*/
			int getUserIndex(Jupiter::IRC::Client::User *user) const;

			/**
			* @brief Fetches the size of the user list.
			*
			* @return Size of the user list.
			*/
			unsigned int getUserCount() const;

			/**
			* @brief Returns the number of channels.
			*
			* @return Number of channels.
			*/
			unsigned int getChannelCount() const;

			/**
			* @brief Returns a channel.
			*
			* @param index Index of the chanel to return.
			* @return The channel at the specified index.
			*/
			Channel *getChannel(unsigned int index) const;

			/**
			* @brief Returns a channel.
			*
			* @param chanName String containing the name of a channel.
			* @return A channel with the specified name if it exists, nullptr otherwise.
			*/
			Channel *getChannel(const Jupiter::ReadableString &chanName) const;

			/**
			* @brief Returns a channel's name at index.
			*
			* @param index Index of the channel.
			* @return Channel's name.
			*/
			const Jupiter::ReadableString &getChannelName(unsigned int index) const;

			/**
			* @brief Returns the index of the channel with a given name.
			*
			* @param chanName String containing the name of a channel.
			* @return Index of a channel if a match is found, -1 otherwise.
			*/
			int getChannelIndex(const Jupiter::ReadableString &chanName) const;

			/**
			* @brief Sends a join request.
			*
			* @param channel Channel to join.
			*/
			void joinChannel(const Jupiter::ReadableString &channel);

			/**
			* @brief Sends a join request with a password.
			*
			* @param channel Channel to join.
			* @param password Password to use.
			*/
			void joinChannel(const Jupiter::ReadableString &channel, const Jupiter::ReadableString &password);

			/**
			* @brief Parts a channel.
			*
			* @param channel Channel to part.
			*/
			void partChannel(const Jupiter::ReadableString &channel);

			/**
			* @brief Parts a channel.
			*
			* @param channel Channel to part.
			* @param message Reason for parting.
			*/
			void partChannel(const Jupiter::ReadableString &channel, const Jupiter::ReadableString &message);

			/**
			* @brief Gets the access level of a user.
			*
			* @param chan String containing a channel name to get the access level from.
			* @param nick String containing the nickname of the user.
			* @return Access level of the user.
			*/
			int getAccessLevel(const Jupiter::ReadableString &chan, const Jupiter::ReadableString &nick) const;

			/**
			* @brief Sends a message.
			*
			* @param dest String containing the destination of the message (nickname or channel).
			* @param message String containing the message to send.
			*/
			void sendMessage(const Jupiter::ReadableString &dest, const Jupiter::ReadableString &message);

			/**
			* @brief Sends a notice.
			*
			* @param dest String containing the destination of the message (nickname or channel).
			* @param message String containing the message to send.
			*/
			void sendNotice(const Jupiter::ReadableString &dest, const Jupiter::ReadableString &message);

			/**
			* @brief Sends a message to all channels of a given type.
			*
			* @param type Type of channel to messasge.
			* @param message String containing the message to send.
			* @return Number of messages sent.
			*/
			unsigned int messageChannels(int type, const Jupiter::ReadableString &message);

			/**
			* @brief Sends a message to all channels with a type of at least 0.
			*
			* @param message String containing the message to send.
			* @return Number of messages sent.
			*/
			unsigned int messageChannels(const Jupiter::ReadableString &message);

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
			void setAutoReconnect(int autoReconnect);

			/**
			* @brief Sends data to the server.
			* Endlines are automatically added.
			*
			* @param rawMessage String containing the data to send.
			*/
			void send(const Jupiter::ReadableString &rawMessage);

			/**
			* @brief Processes an input line of IRC protocol data.
			*
			* @return 0 upon success, an error code otherwise.
			*/
			int process_line(const Jupiter::ReadableString &line);

			/**
			* @brief Returns a key's value.
			* This reads from the client's config section first, then default if it doesn't exist.
			*
			* @param key String containing the key name.
			* @param defaultValue Optional parameter specifying the default value to return if none is found.
			* @return String containing the key value if it exists, defaultValue otherwise.
			*/
			const Jupiter::ReadableString &readConfigValue(const Jupiter::ReadableString &key, const Jupiter::ReadableString &defaultValue = Jupiter::ReferenceString::empty) const;

			/**
			* @brief Returns a key's value as a boolean.
			* This reads from the client's config section first, then default if it doesn't exist.
			*
			* @param key String containing the key name.
			* @return Boolean value of the key value if it exists, defaultValue otherwise.
			*/
			bool readConfigBool(const Jupiter::ReadableString &key, bool defaultValue = false) const;

			/**
			* @brief Returns a key's value as an integer.
			* This reads from the client's config section first, then default if it doesn't exist.
			*
			* @param key String containing the key name.
			* @return Integer value of the key value if it exists, defaultValue otherwise.
			*/
			int readConfigInt(const Jupiter::ReadableString &key, int defaultValue = 0) const;

			/**
			* @brief Returns a key's value as a long integer.
			* This reads from the client's config section first, then default if it doesn't exist.
			*
			* @param key String containing the key name.
			* @return Long integer value of the key value if it exists, defaultValue otherwise.
			*/
			long readConfigLong(const Jupiter::ReadableString &key, long defaultValue = 0) const;

			/**
			* @brief Returns a key's value as a double.
			* This reads from the client's config section first, then default if it doesn't exist.
			*
			* @param key String containing the key name.
			* @return Double value of the key value if it exists, defaultValue otherwise.
			*/
			double readConfigDouble(const Jupiter::ReadableString &key, double defaultValue = 0) const;

			/**
			* @brief Writes to the server's log file.
			*
			* @param message String containing the text to write to the file.
			*/
			void writeToLogs(const Jupiter::ReadableString &message);

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
			void disconnect(bool stayDead = false);

			/**
			* @brief Completely disconnects the client from a server.
			*
			* @param message String containing the QUIT message to send prior to disconnecting.
			*/
			void disconnect(const Jupiter::ReadableString &message, bool stayDead = false);

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
			Client(const Jupiter::Config *in_primary_section, const Jupiter::Config *in_secondary_section);

			/**
			* @brief Destructor for a client.
			*/
			virtual ~Client();

		/** Private members */
		private:
			struct Data;
			Data *data_;
		}; // Jupiter::IRC::Client class

	} // Jupiter::IRC namespace

} // Jupiter namespace

#endif // _IRC_CLIENT_H_HEADER