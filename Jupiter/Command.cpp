/**
 * Copyright (C) 2013-2015 Jessica James.
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