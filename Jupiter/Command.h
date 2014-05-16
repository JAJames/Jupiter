/**
 * Copyright (C) 2013-2014 Justin James.
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

#if !defined _COMMAND_H_HEADER
#define _COMMAND_H_HEADER

/**
 * @file Command.h
 * @brief Provides an extendable command system.
 */

#include "Jupiter.h"
#include "ArrayList.h"
#include "CString.h"

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
		void addTrigger(const char *trigger);

		/**
		* @brief Fetches a command's specified trigger.
		*
		* @param index Index of the trigger to return.
		* @return Trigger of the command at the specified index.
		*/
		const char *getTrigger(short index = 0) const;

		/**
		* @brief Returns the number of triggers accepted by the command.
		*
		* @return Number of triggers the command accepts.
		*/
		unsigned int getTriggerCount() const;

		/**
		* @brief Checks if a specified trigger matches one of the stored triggers.
		* Note: Case insensitive.
		*
		* @param trigger Trigger to check against the trigger list.
		* @return True if a match was found, false otherwise.
		*/
		bool matches(const char *trigger) const;

		/**
		* @brief Returns a brief explanation and syntax description about a command.
		*
		* @return Brief description of command and syntax.
		*/
		virtual const char *getHelp() = 0;

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
		Data *data_;
	};

}

#endif // _COMMAND_H_HEADER