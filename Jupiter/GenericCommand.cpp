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

#include "GenericCommand.h"
#include "Plugin.h"

Jupiter::ArrayList<Jupiter::GenericCommand> o_genericCommands;
Jupiter::ArrayList<Jupiter::GenericCommand> *Jupiter::g_generic_commands = &o_genericCommands;

/** GenericCommand */

Jupiter::GenericCommand::GenericCommand()
{
	o_genericCommands.add(this);

	for (size_t index = 0; index != Jupiter::plugins->size(); ++index)
		Jupiter::plugins->get(index)->OnGenericCommandAdd(*this);
}

Jupiter::GenericCommand::~GenericCommand()
{
	size_t count = o_genericCommands.size();
	while (count != 0)
		if (o_genericCommands.get(--count) == this)
		{
			o_genericCommands.remove(count);
			break;
		}

	for (size_t index = 0; index != Jupiter::plugins->size(); ++index)
		Jupiter::plugins->get(index)->OnGenericCommandRemove(*this);
}

/** GenericCommand::ResponseLine */

Jupiter::GenericCommand::ResponseLine::ResponseLine(const Jupiter::ReadableString &response_, GenericCommand::DisplayType type_)
{
	GenericCommand::ResponseLine::response = response_;
	GenericCommand::ResponseLine::type = type_;
}

Jupiter::GenericCommand::ResponseLine *Jupiter::GenericCommand::ResponseLine::set(const Jupiter::ReadableString &response_, GenericCommand::DisplayType type_)
{
	GenericCommand::ResponseLine::response = response_;
	GenericCommand::ResponseLine::type = type_;
	return this;
}

Jupiter::GenericCommand *Jupiter::getGenericCommand(const Jupiter::ReadableString &trigger)
{
	size_t count = o_genericCommands.size();
	while (count != 0)
	{
		Jupiter::GenericCommand *cmd = o_genericCommands.get(--count);
		if (cmd->matches(trigger))
			return cmd;
	}
	return nullptr;
}
