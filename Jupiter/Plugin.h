/**
 * Copyright (C) 2014 Justin James.
 *
 * This license must be preserved.
 * Any applications, libraries, or code which make any use of any
 * component of this program must not be commercial, unless explicit
 * permission is granted from the original author. The use of this
 * program for non-profit purposes is permitted.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In the event that this license restricts you from making desired use of this program, contact the original author.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#if !defined _PLUGIN_H_HEADER
#define _PLUGIN_H_HEADER

/**
 * @file Plugin.h
 * @brief Provides a hot-swapable plugin system.
 */

#include "ArrayList.h"
#include "Thinker.h"
#include "CString.h"
#include "Rehash.h"

namespace Jupiter
{
	/** Forward declaration */
	namespace IRC { class Client; }

	/**
	* @brief Provides the basis for plugin interfacing.
	*/
	class JUPITER_API Plugin : public Jupiter::Thinker, public Jupiter::Rehashable
	{
	public:
		/**
		* @brief This is called when a connection has been successfully established.
		* The current implementation calls this after the MOTD.
		*/
		virtual void OnConnect(Jupiter::IRC::Client *server);

		/**
		* @brief This is called when at the end of disconnect().
		* This is called upon any connection failure or error.
		*/
		virtual void OnDisconnect(Jupiter::IRC::Client *server);

		/**
		* @brief This is called when at the end of reconnect().
		*
		* @param successConnect Used to determine if a connection was successfully established.
		* @see connect().
		*/
		virtual void OnReconnectAttempt(Jupiter::IRC::Client *server, bool successConnect);

		/**
		* @brief This is called when rehash() is called.
		* @see Jupiter::Rehashable::OnRehash().
		*
		* @return 0 if no error occurs, a postive integer if an error occurs, or a negative integer if an error occurs and the object should be deleted.
		*/
		virtual int OnRehash();

		/**
		* @brief This is called after a message has been normally processed.
		* This always happens last. All other hooks will be called before this.
		*
		* @param raw The raw message.
		*/
		virtual void OnRaw(Jupiter::IRC::Client *server, const Jupiter::StringType &raw);

		/**
		* @brief This is called after an IRC numeric has been processed.
		*
		* @param raw The raw message.
		*/
		virtual void OnNumeric(Jupiter::IRC::Client *server, long int numeric, const Jupiter::StringType &raw);

		/**
		* @brief This is called when an ERROR is received.
		* This indicates a connection termination, and thus, disconnect() is called immediately after this.
		*
		* @param message Message sent by the server.
		*/
		virtual void OnError(Jupiter::IRC::Client *server, const Jupiter::StringType &message);

		/**
		* @brief This is called when a chat message is received.
		*
		* @param channel String containing the destination of the message.
		* @param nick String containing the nickname of the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnChat(Jupiter::IRC::Client *server, const Jupiter::StringType &channel, const Jupiter::StringType &nick, const Jupiter::StringType &message);

		/**
		* @brief This is called when a notice is received.
		*
		* @param chan String containing the destination of the message.
		* @param nick String containing the nickname of the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnNotice(Jupiter::IRC::Client *server, const Jupiter::StringType &chan, const Jupiter::StringType &sender, const Jupiter::StringType &message);

		/**
		* @brief This is called when a server notice is received.
		*
		* @param chan String containing the destination of the message.
		* @param nick String containing the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnServerNotice(Jupiter::IRC::Client *server, const Jupiter::StringType &chan, const Jupiter::StringType &sender, const Jupiter::StringType &message);

		/**
		* @brief This is called when a CTCP message is received.
		*
		* @param channel String containing the destination of the message.
		* @param nick String containing the nickname of the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnCTCP(Jupiter::IRC::Client *server, const Jupiter::StringType &channel, const Jupiter::StringType &nick, const Jupiter::StringType &message);

		/**
		* @brief This is called when an action message is received.
		*
		* @param chan String containing the destination of the message.
		* @param nick String containing the nickname of the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnAction(Jupiter::IRC::Client *server, const Jupiter::StringType &chan, const Jupiter::StringType &nick, const Jupiter::StringType &message);

		/**
		* @brief This is called when an invite is received.
		*
		* @param chan String containing the destination of the message.
		* @param inviter String containing the nickname of the inviter.
		* @param invited String containing the nickname of the user invited.
		*/
		virtual void OnInvite(Jupiter::IRC::Client *server, const Jupiter::StringType &chan, const Jupiter::StringType &inviter, const Jupiter::StringType &invited);

		/**
		* @brief This is called when a chat message is received.
		*
		* @param chan String containing the destination of the message.
		* @param nick String containing the nickname of the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnJoin(Jupiter::IRC::Client *server, const Jupiter::StringType &chan, const Jupiter::StringType &nick);

		/**
		* @brief This is called when a user parts a channel.
		*
		* @param chan String containing the channel parted from.
		* @param nick String containing the nickname of the user.
		* @param reason String containing the reason for parting, or nullptr if none is specified.
		*/
		virtual void OnPart(Jupiter::IRC::Client *server, const Jupiter::StringType &chan, const Jupiter::StringType &nick, const Jupiter::StringType &reason);

		/**
		* @brief This is called when a user changes their nickname.
		*
		* @param oldnick String containing the old nickname of the user.
		* @param newnick String containing the new nickname of the user.
		*/
		virtual void OnNick(Jupiter::IRC::Client *server, const Jupiter::StringType &oldnick, const Jupiter::StringType &newnick);

		/**
		* @brief This is called when a user is kicked from a channel.
		*
		* @param chan String containing the channel kicked from.
		* @param kicker String containing the nickname of the kicker.
		* @param kicked String containing the nickname of the user kicked.
		* @param reason String containing the reason for the kick, or nullptr if none is specified.
		*/
		virtual void OnKick(Jupiter::IRC::Client *server, const Jupiter::StringType &chan, const Jupiter::StringType &kicker, const Jupiter::StringType &kicked, const Jupiter::StringType &reason);

		/**
		* @brief This is called when a user quits the server.
		*
		* @param nick String containing the nickname of the user.
		* @param message String containing the reason for quiting.
		*/
		virtual void OnQuit(Jupiter::IRC::Client *server, const Jupiter::StringType &nick, const Jupiter::StringType &message);

		/**
		* @brief This is called when a channel mode is changed.
		*
		* @param chan String containing the affected channel.
		* @param nick String containing the nickname of the user.
		* @param modeString String containing the modes changed.
		*/
		virtual void OnMode(Jupiter::IRC::Client *server, const Jupiter::StringType &chan, const Jupiter::StringType &nick, const Jupiter::StringType &modeString);

		/**
		* @brief This is called when a server "thinks".
		*
		* @param server Server that's thinking.
		*/
		virtual void OnThink(Jupiter::IRC::Client *server);

		/**
		* @brief Makes the Plugin "think". This does nothing, unless overloaded.
		* This method should return an error other than 0 if you want the plugin to be released.
		*
		* @return 0 when no error occurs, an error otherwise.
		*/
		virtual int think();

		/**
		* @brief Returns the name of the plugin.
		*
		* @return String containing the name of the plugin.
		*/
		virtual const char *getName() = 0;

		/**
		* @brief Destructor for Plugin class.
		*/
		virtual ~Plugin();
	};

	/** Plugin management functions */

	/** The list containing pointers to plugins */
	JUPITER_API extern Jupiter::ArrayList<Jupiter::Plugin> *plugins;

	/**
	* @brief Sets the directory to look in for plugins.
	* Note: This DOES NOT copy the input string.
	*
	* @param dir Directory to look for plugins in.
	*/
	JUPITER_API void setPluginDirectory(const char *dir);

	/**
	* @brief Returns the current plugin directory.
	*
	* @return String containing the directory which is prepended to module load attempts.
	*/
	JUPITER_API const char *getPluginDirectory();

	/**
	* @brief Loads a module, appending .so or .dll as appropriate for the operating system.
	*
	* @param pluginName The name of the plugin to load.
	* @return A pointer to the plugin that was loaded on success, nullptr otherwise.
	*/
	JUPITER_API Jupiter::Plugin *loadPlugin(const char *pluginName);

	/**
	* @brief Loads a module based on a true file name.
	*
	* @param file File to attempt to load as a plugin.
	* @return A pointer to the plugin that was loaded on success, nullptr otherwise.
	*/
	JUPITER_API Jupiter::Plugin *loadPluginFile(const char *file);

	/**
	* @brief Unloads a module and removes it from the module list, based on its index.
	*
	* @param index Index of the module.
	* @return True if a module was unloaded, false otherwise.
	*/
	JUPITER_API bool freePlugin(unsigned int index);

	/**
	* @brief Unloads a module and removes it from the module list, based on its data.
	*
	* @param plugin Pointer to the plugin to unload.
	* @return True if a module was unloaded, false otherwise.
	*/
	JUPITER_API bool freePlugin(Jupiter::Plugin *plugin);

	/**
	* @brief Unloads a module and removes it from the module list, based on its name.
	* Note: This function accepts wildcard strings.
	*
	* @param pluginName Name of the module to unload.
	* @return True if a module was unloaded, false otherwise.
	*/
	JUPITER_API bool freePlugin(const char *pluginName);

	/**
	* @brief Fetches a plugin from the list and returns it, based on its index.
	*
	* @param index Index of the module to return.
	* @return A module on success, nullptr otherwise.
	*/
	JUPITER_API Jupiter::Plugin *getPlugin(unsigned int index);

	/**
	* @brief Fetches a plugin from the list and returns it, based on its name.
	*
	* @param pluginName String containing the name of the plugin.
	* @return A module on success, nullptr otherwise.
	*/
	JUPITER_API Jupiter::Plugin *getPlugin(const char *pluginName);
}

#endif // _PLUGIN_H_HEADER