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

#include <cstring>
#include "ArrayList.h"
#include "Functions.h"
#include "Command.h"

struct Jupiter::Command::Data
{
public:
	Jupiter::ArrayList<Jupiter::CString_Strict<char>> triggers;
};

Jupiter::Command::Command()
{
	Jupiter::Command::data_ = new Jupiter::Command::Data();
}

Jupiter::Command::Command(const Command &command)
{
	Jupiter::Command::data_ = new Jupiter::Command::Data();
	Jupiter::CStringS *trigger;
	for (int i = command.data_->triggers.size() - 1; i >= 0; i--)
	{
		trigger = new Jupiter::CStringS(*command.data_->triggers.get(i));
		Jupiter::Command::data_->triggers.add(trigger);
	}
}

Jupiter::Command::~Command()
{
	for (int i = Jupiter::Command::data_->triggers.size() - 1; i >= 0; i--) delete Jupiter::Command::data_->triggers.remove(i);
	delete Jupiter::Command::data_;
}

// Command Functions

void Jupiter::Command::addTrigger(const char *trigger)
{
	Jupiter::CStringS *aTrigger = new Jupiter::CStringS(trigger);
	Jupiter::Command::data_->triggers.add(aTrigger);
}

const char *Jupiter::Command::getTrigger(short index) const
{
	return Jupiter::Command::data_->triggers.get(index)->c_str();
}

unsigned int Jupiter::Command::getTriggerCount() const
{
	return Jupiter::Command::data_->triggers.size();
}

bool Jupiter::Command::matches(const char *trigger) const
{
	unsigned int size = Jupiter::Command::data_->triggers.size();
	for (unsigned int i = 0; i < size; i++) if (Jupiter::Command::data_->triggers.get(i)->equalsi(trigger)) return true;
	return false;
}
