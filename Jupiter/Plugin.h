/**
 * Copyright (C) 2014-2016 Jessica James.
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

#if !defined _PLUGIN_H_HEADER
#define _PLUGIN_H_HEADER

/**
 * @file Plugin.h
 * @brief Provides a hot-swapable plugin system.
 */

#include "ArrayList.h"
#include "Thinker.h"
#include "Rehash.h"
#include "INIConfig.h"
#include "String.h"

/** DLL Linkage Nagging */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif // _MSC_VER

namespace Jupiter
{
	/** Forward declaration */
	namespace IRC { class Client; }
	class GenericCommand;

	/**
	* @brief Provides the basis for plugin interfacing.
	*/
	class JUPITER_API Plugin : public Jupiter::Thinker, public Jupiter::Rehashable
	{
	public: // Jupiter::Thinker
		/**
		* @brief Makes the Plugin "think". This does nothing, unless overloaded.
		* This method should return an error other than 0 if you want the plugin to be released.
		*
		* @return 0 when no error occurs, an error otherwise.
		*/
		virtual int think() override;

	public: // Jupiter::Rehashable
		/**
		* @brief This is called when rehash() is called.
		* @see Jupiter::Rehashable::OnRehash().
		*
		* @return 0 if no error occurs, a postive integer if an error occurs, or a negative integer if an error occurs and the object should be deleted.
		*/
		virtual int OnRehash() override;

		/**
		* @brief This is called when OnRehash() returns an error during rehash().
		* @see Jupiter::Rehashable::OnRehash().
		* Note: If you override this, be sure to call Jupiter::Plugin::OnBadRehash(removed) before returning.
		*
		* @param removed True if the object was removed from the rehash list, false otherwise.
		* @return True if the object should be deleted, false otherwise.
		*/
		virtual bool OnBadRehash(bool removed) override;

	public: // Jupiter::Plugin
		/**
		* @brief Checks if this plugin should be unloaded.
		* This returns "true" after a call to OnBadRehash().
		*
		* @returns True if this plugin should be unloaded, false otherwise.
		*/
		bool shouldRemove() const;

		/**
		* @brief Returns the name of the plugin.
		*
		* @return String containing the name of the plugin.
		*/
		const Jupiter::ReadableString &getName() const;

		/**
		* @brief Returns the plugin's configuration file.
		*
		* @return Plugin's configuration file.
		*/
		Jupiter::Config &getConfig() const;

		/**
		* @brief Initializes the plugin.
		* Note: This function is executed immediately AFTER 'name' is assigned and 'config' is loaded.
		*
		* @return True on success, false otherwise.
		*/
		virtual bool initialize();

		/**
		* @brief This is called after all plugins have been initialized during application startup
		* Note: There is no guarantee that this will be called, AND there is no guarantee that more plugins will not be loaded later
		*/
		virtual void OnPostInitialize();

		/** IRC Listeners */

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
		* @brief This is called after a message has been normally processed.
		* This always happens last. All other hooks will be called before this.
		*
		* @param raw The raw message.
		*/
		virtual void OnRaw(Jupiter::IRC::Client *server, const Jupiter::ReadableString &raw);

		/**
		* @brief This is called after an IRC numeric has been processed.
		*
		* @param raw The raw message.
		*/
		virtual void OnNumeric(Jupiter::IRC::Client *server, long int numeric, const Jupiter::ReadableString &raw);

		/**
		* @brief This is called when an ERROR is received.
		* This indicates a connection termination, and thus, disconnect() is called immediately after this.
		*
		* @param message Message sent by the server.
		*/
		virtual void OnError(Jupiter::IRC::Client *server, const Jupiter::ReadableString &message);

		/**
		* @brief This is called when a chat message is received.
		*
		* @param channel String containing the destination of the message.
		* @param nick String containing the nickname of the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnChat(Jupiter::IRC::Client *server, const Jupiter::ReadableString &channel, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &message);

		/**
		* @brief This is called when a notice is received.
		*
		* @param chan String containing the destination of the message.
		* @param nick String containing the nickname of the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnNotice(Jupiter::IRC::Client *server, const Jupiter::ReadableString &chan, const Jupiter::ReadableString &sender, const Jupiter::ReadableString &message);

		/**
		* @brief This is called when a server notice is received.
		*
		* @param chan String containing the destination of the message.
		* @param nick String containing the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnServerNotice(Jupiter::IRC::Client *server, const Jupiter::ReadableString &chan, const Jupiter::ReadableString &sender, const Jupiter::ReadableString &message);

		/**
		* @brief This is called when a CTCP message is received.
		*
		* @param channel String containing the destination of the message.
		* @param nick String containing the nickname of the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnCTCP(Jupiter::IRC::Client *server, const Jupiter::ReadableString &channel, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &message);

		/**
		* @brief This is called when an action message is received.
		*
		* @param chan String containing the destination of the message.
		* @param nick String containing the nickname of the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnAction(Jupiter::IRC::Client *server, const Jupiter::ReadableString &chan, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &message);

		/**
		* @brief This is called when an invite is received.
		*
		* @param chan String containing the destination of the message.
		* @param inviter String containing the nickname of the inviter.
		* @param invited String containing the nickname of the user invited.
		*/
		virtual void OnInvite(Jupiter::IRC::Client *server, const Jupiter::ReadableString &chan, const Jupiter::ReadableString &inviter, const Jupiter::ReadableString &invited);

		/**
		* @brief This is called when a chat message is received.
		*
		* @param chan String containing the destination of the message.
		* @param nick String containing the nickname of the sender.
		* @param message String containing the message sent.
		*/
		virtual void OnJoin(Jupiter::IRC::Client *server, const Jupiter::ReadableString &chan, const Jupiter::ReadableString &nick);

		/**
		* @brief This is called when a user parts a channel.
		*
		* @param chan String containing the channel parted from.
		* @param nick String containing the nickname of the user.
		* @param reason String containing the reason for parting, or nullptr if none is specified.
		*/
		virtual void OnPart(Jupiter::IRC::Client *server, const Jupiter::ReadableString &chan, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &reason);

		/**
		* @brief This is called when a user changes their nickname.
		*
		* @param oldnick String containing the old nickname of the user.
		* @param newnick String containing the new nickname of the user.
		*/
		virtual void OnNick(Jupiter::IRC::Client *server, const Jupiter::ReadableString &oldnick, const Jupiter::ReadableString &newnick);

		/**
		* @brief This is called when a user is kicked from a channel.
		*
		* @param chan String containing the channel kicked from.
		* @param kicker String containing the nickname of the kicker.
		* @param kicked String containing the nickname of the user kicked.
		* @param reason String containing the reason for the kick, or nullptr if none is specified.
		*/
		virtual void OnKick(Jupiter::IRC::Client *server, const Jupiter::ReadableString &chan, const Jupiter::ReadableString &kicker, const Jupiter::ReadableString &kicked, const Jupiter::ReadableString &reason);

		/**
		* @brief This is called when a user quits the server.
		*
		* @param nick String containing the nickname of the user.
		* @param message String containing the reason for quiting.
		*/
		virtual void OnQuit(Jupiter::IRC::Client *server, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &message);

		/**
		* @brief This is called when a channel mode is changed.
		*
		* @param chan String containing the affected channel.
		* @param nick String containing the nickname of the user.
		* @param modeString String containing the modes changed.
		*/
		virtual void OnMode(Jupiter::IRC::Client *server, const Jupiter::ReadableString &chan, const Jupiter::ReadableString &nick, const Jupiter::ReadableString &modeString);

		/**
		* @brief This is called when a server "thinks".
		*
		* @param server Server that's thinking.
		*/
		virtual void OnThink(Jupiter::IRC::Client *server);

		/**
		* @brief This is called when a GenericCommand is instantiated.
		*/
		virtual void OnGenericCommandAdd(Jupiter::GenericCommand &command);

		/**
		* @brief This is called when a GenericCommand is deleted.
		*/
		virtual void OnGenericCommandRemove(Jupiter::GenericCommand &command);

		/**
		* @brief Constructor for the Plugin class.
		*/
		Plugin();

		/**
		* @brief Destructor for Plugin class.
		*/
		virtual ~Plugin();

	public: // Plugin management functions
		/**
		* @brief Sets the directory to look in for plugins.
		*
		* @param dir Directory to look for plugins in.
		*/
		static void setDirectory(const Jupiter::ReadableString &dir);

		/**
		* @brief Returns the current plugin directory.
		*
		* @return String containing the directory which is prepended to module load attempts.
		*/
		static const Jupiter::ReadableString &getDirectory();

		/**
		* @brief Sets the directory to look in for plugin configuration files.
		*
		* @param dir Directory to look for configuration files in.
		*/
		static void setConfigDirectory(const Jupiter::ReadableString &dir);

		/**
		* @brief Returns the current plugin configuration directory
		*
		* @return String containing the directory which is prepended to configuration load attempts.
		*/
		static const Jupiter::ReadableString &getConfigDirectory();

		/**
		* @brief Loads a module, appending .so or .dll as appropriate for the operating system.
		*
		* @param pluginName The name of the plugin to load.
		* @return A pointer to the plugin that was loaded on success, nullptr otherwise.
		*/
		static Jupiter::Plugin *load(const Jupiter::ReadableString &pluginName);

		/**
		* @brief Unloads a module and removes it from the module list, based on its index.
		*
		* @param index Index of the module.
		* @return True if a module was unloaded, false otherwise.
		*/
		static bool free(size_t index);

		/**
		* @brief Unloads a module and removes it from the module list, based on its data.
		*
		* @param plugin Pointer to the plugin to unload.
		* @return True if a module was unloaded, false otherwise.
		*/
		static bool free(Jupiter::Plugin *plugin);

		/**
		* @brief Unloads a module and removes it from the module list, based on its name.
		* Note: This function accepts wildcard strings.
		*
		* @param pluginName Name of the module to unload.
		* @return True if a module was unloaded, false otherwise.
		*/
		static bool free(const Jupiter::ReadableString &pluginName);

		/**
		* @brief Fetches a plugin from the list and returns it, based on its index.
		*
		* @param index Index of the module to return.
		* @return A module on success, nullptr otherwise.
		*/
		static Jupiter::Plugin *get(size_t index);

		/**
		* @brief Fetches a plugin from the list and returns it, based on its name.
		*
		* @param pluginName String containing the name of the plugin.
		* @return A module on success, nullptr otherwise.
		*/
		static Jupiter::Plugin *get(const Jupiter::ReadableString &pluginName);

	protected:
		bool _shouldRemove = false;
		Jupiter::StringS name;
		Jupiter::INIConfig config;
	};

	/** The list containing pointers to plugins */
	JUPITER_API extern Jupiter::ArrayList<Jupiter::Plugin> *plugins;
}

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // _PLUGIN_H_HEADER