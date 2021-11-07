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

#if !defined _COMMAND_H_HEADER
#define _COMMAND_H_HEADER

/**
 * @file Command.h
 * @brief Provides an extendable command system.
 */

#include "Jupiter.h"
#include "Reference_String.h"

namespace Jupiter
{

	/**
	* @brief Provides the basis of a command system.
	*/
	class JUPITER_API Command
	{
	public:
		/**
		* @brief Adds a trigger to the command.
		*
		* @param trigger Trigger to add to the command.
		*/
		void addTrigger(const Jupiter::ReadableString &trigger);

		/**
		* @brief Fetches a command's specified trigger.
		*
		* @param index Index of the trigger to return.
		* @return Trigger of the command at the specified index.
		*/
		const Jupiter::ReadableString &getTrigger(size_t index = 0) const;

		/**
		* @brief Returns the number of triggers accepted by the command.
		*
		* @return Number of triggers the command accepts.
		*/
		size_t getTriggerCount() const;

		/**
		* @brief Checks if a specified trigger matches one of the stored triggers.
		* Note: Case insensitive.
		*
		* @param trigger Trigger to check against the trigger list.
		* @return True if a match was found, false otherwise.
		*/
		bool matches(const Jupiter::ReadableString &trigger) const;

		/**
		* @brief Returns a brief explanation and syntax description about a command.
		*
		* @param parameters Optional string containing any data to be passed to the help command.
		* @return Brief description of command and syntax.
		*/
		virtual const Jupiter::ReadableString &getHelp(const Jupiter::ReadableString &parameters = Jupiter::ReferenceString::empty) = 0;

		/**
		* @brief Default constructor for command class.
		*/
		Command();

		/**
		* @brief Copy constructor for command class.
		*/
		Command(const Command &command);

		/**
		* @brief destructor for command class.
		*/
		virtual ~Command();

		/** Private members */
	private:
		struct Data;
		Data *m_data;
	};

}

#endif // _COMMAND_H_HEADER