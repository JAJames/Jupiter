/**
 * Copyright (C) Justin James - All Rights Reserved.
 * Unauthorized use or copying of this file via any medium is strictly prohibited.
 * This document is proprietary and confidential.
 * This document should be immediately destroyed unless given explicit permission by Justin James.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include <cstring>
#include "ArrayList.h"
#include "Functions.h"
#include "Command.h"
#include "String.h"

struct Jupiter::Command::Data
{
public:
	Jupiter::ArrayList<Jupiter::StringS> triggers;
};

Jupiter::Command::Command()
{
	Jupiter::Command::data_ = new Jupiter::Command::Data();
}

Jupiter::Command::Command(const Command &command)
{
	Jupiter::Command::data_ = new Jupiter::Command::Data();
	for (size_t i = 0; i != Jupiter::Command::data_->triggers.size(); i++)
		Jupiter::Command::data_->triggers.add(new Jupiter::StringS(*command.data_->triggers.get(i)));
}

Jupiter::Command::~Command()
{
	Jupiter::Command::data_->triggers.emptyAndDelete();
	delete Jupiter::Command::data_;
}

// Command Functions

void Jupiter::Command::addTrigger(const Jupiter::ReadableString &trigger)
{
	Jupiter::Command::data_->triggers.add(new Jupiter::StringS(trigger));
}

const Jupiter::ReadableString &Jupiter::Command::getTrigger(size_t index) const
{
	return *Jupiter::Command::data_->triggers.get(index);
}

size_t Jupiter::Command::getTriggerCount() const
{
	return Jupiter::Command::data_->triggers.size();
}

bool Jupiter::Command::matches(const Jupiter::ReadableString &trigger) const
{
	for (size_t i = 0; i != Jupiter::Command::data_->triggers.size(); i++)
		if (Jupiter::Command::data_->triggers.get(i)->equalsi(trigger)) return true;
	return false;
}