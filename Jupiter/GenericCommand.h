/**
 * Copyright (C) 2015-2016 Jessica James.
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

#include "Command.h"
#include "String.h"

/** DLL Linkage Nagging */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace Jupiter
{
	/**
	* @brief Provides the base for generic commands.
	*/
	class JUPITER_API GenericCommand : public Jupiter::Command
	{
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
		* @brief Default constructor for the generic command class.
		*/
		GenericCommand();

		/**
		* @brief Destructor for the generic command class.
		*/
		virtual ~GenericCommand();
	};

	/**
	* @brief Fetches a generic command based on its trigger.
	*
	* @param trigger Trigger of the command to fetch.
	* @return A generic command if it exists, nullptr otherwise.
	*/
	JUPITER_API extern GenericCommand *getGenericCommand(const Jupiter::ReadableString &trigger);

	/** Generic command list */
	JUPITER_API extern Jupiter::ArrayList<GenericCommand> *g_generic_commands;
}

/** Generic Command Macros */

/** Defines the core of a generic command's declaration. This should be included in every generic command. */
#define BASE_GENERIC_COMMAND(CLASS) \
	public: \
	CLASS(); \
	GenericCommand::ResponseLine *trigger(const Jupiter::ReadableString &parameters); \
	const Jupiter::ReadableString &getHelp(const Jupiter::ReadableString &parameters); \
	static CLASS instance;

/** Expands to become the entire declaration for a generic command. In most cases, this will be sufficient. */
#define GENERIC_GENERIC_COMMAND(CLASS) \
class CLASS : public GenericCommand { \
	BASE_GENERIC_COMMAND(CLASS) \
};

/** Instantiates a generic command. */
#define GENERIC_COMMAND_INIT(CLASS) \
	CLASS CLASS :: instance = CLASS (); \
	CLASS & CLASS ## _instance = CLASS :: instance;

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // _GENERICCOMMAND_H_HEADER
