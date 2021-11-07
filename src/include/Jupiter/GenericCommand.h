/**
 * Copyright (C) 2015-2021 Jessica James.
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

#if !defined _GENERICCOMMAND_H_HEADER
#define _GENERICCOMMAND_H_HEADER

/**
* @file GenericCommand.h
* @brief Provides an extendable command system.
*/

#include <memory>
#include "Command.h"
#include "String.hpp"

/** DLL Linkage Nagging */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace Jupiter
{
	class GenericCommandNamespace;

	/**
	* @brief Provides the base for generic commands.
	*/
	class JUPITER_API GenericCommand : public Jupiter::Command, public std::enable_shared_from_this<GenericCommand> {
		// Note: only needs shared_from_this because of the horrendous way this was originally written before shared_ptr
		// This isn't worth improving since it should be replaced eventually
	public:
		/** Enumerated class to guide output in generic command interpreters */
		enum class DisplayType
		{
			PublicSuccess,
			PrivateSuccess,
			PublicError,
			PrivateError,
		};

		/** Data entry returned by trigger() */
		struct JUPITER_API ResponseLine
		{
			Jupiter::StringS response;
			GenericCommand::DisplayType type;
			ResponseLine *next = nullptr;

			/**
			* @brief Sets the response and type of the ResponseLine.
			*
			* @param in_response Value to set response to.
			* @param in_type Value to set type to.
			* @return This.
			*/
			ResponseLine *set(const Jupiter::ReadableString &response, GenericCommand::DisplayType type);
			ResponseLine() = default;
			ResponseLine(const Jupiter::ReadableString &response, GenericCommand::DisplayType type);
		};

		/**
		* @brief Called when the command is to be executed.
		*
		* @param input Parameters passed to the command by the user.
		*/
		virtual ResponseLine *trigger(const Jupiter::ReadableString &input) = 0;

		/**
		* @brief Checks if this command is a namespace
		*
		* @return True if the command is a namespace, false otherwise
		*/
		virtual bool isNamespace() const;

		/**
		* @brief Places this command in a namespace, creating new namespaces as necessary
		* Note: This moves the command into a namespace relative to its current namespace
		*
		* @param in_namespace Name of the namespace to move the command into
		*/
		void setNamespace(const Jupiter::ReadableString &in_namespace);

		/**
		* @brief Places this command in a namespace
		*
		* @param in_namespace Namespace to move the command into
		*/
		void setNamespace(GenericCommandNamespace &in_namespace);

		/**
		* @brief Fetches the namespace this command is in
		*
		* @return Namespace this command is in, or nullptr if there is none.
		*/
		GenericCommandNamespace *getNamespace() const;

		/**
		* @brief Default constructor for the generic command class.
		*/
		GenericCommand();

		/**
		* @brief Destructor for the generic command class.
		*/
		virtual ~GenericCommand();

	protected:
		GenericCommandNamespace *m_parent = nullptr;
	};

	/**
	* @brief Provides the base for generic command namespaces
	*/
	class JUPITER_API GenericCommandNamespace : public Jupiter::GenericCommand
	{
	public: // Jupiter::Command
		ResponseLine *trigger(const Jupiter::ReadableString &input) override;
		const Jupiter::ReadableString &getHelp(const Jupiter::ReadableString &parameters) override;

	public: // Jupiter::GenericCommand
		bool isNamespace() const override;

	public: // Jupiter::GenericCommandNamespace
		bool isUsing() const;
		void setUsing(bool in_value);

		std::vector<GenericCommand*> getCommands() const; // differs from m_commands in that it checks if it's using a namespace
		GenericCommand *getCommand(const Jupiter::ReadableString &in_command) const;

		void addCommand(GenericCommand &in_command);
		void removeCommand(GenericCommand &in_command);
		void removeCommand(const Jupiter::ReadableString &in_command);

		void updateHelp();

		virtual ~GenericCommandNamespace();

	protected:
		bool m_using;
		std::vector<GenericCommand*> m_commands;

	private:
		bool m_should_update_help = true;
		Jupiter::StringS m_help;
	};

	/** Generic command list */
	JUPITER_API extern GenericCommandNamespace &g_generic_commands;
}

/** Generic Command Macros */

/** Defines the core of a generic command's declaration. This should be included in every generic command. */
#define BASE_GENERIC_COMMAND(CLASS) \
	public: \
	CLASS(); \
	Jupiter::GenericCommand::ResponseLine *trigger(const Jupiter::ReadableString &parameters) override; \
	const Jupiter::ReadableString &getHelp(const Jupiter::ReadableString &parameters) override; \
	static CLASS &instance;

/** Expands to become the entire declaration for a generic command. In most cases, this will be sufficient. */
#define GENERIC_GENERIC_COMMAND(CLASS) \
class CLASS : public Jupiter::GenericCommand { \
	BASE_GENERIC_COMMAND(CLASS) \
};

/** Instantiates a generic command. */
#define GENERIC_COMMAND_INIT(CLASS) \
class CLASS ## _Init : public CLASS { \
public: \
	CLASS ## _Init() { \
		for (auto& plugin : Jupiter::plugins) \
			plugin->OnGenericCommandAdd(*this); \
	} }; \
	CLASS ## _Init CLASS ## _instance = CLASS ## _Init (); \
	CLASS & CLASS :: instance = CLASS ## _instance;

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // _GENERICCOMMAND_H_HEADER
