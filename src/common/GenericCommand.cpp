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

#include "GenericCommand.h"
#include "Plugin.h"

using namespace Jupiter::literals;

constexpr const char GENERIC_COMMAND_WORD_DELIMITER = ' ';
constexpr const char *GENERIC_COMMAND_WORD_DELIMITER_CS = " ";

Jupiter::GenericCommandNamespace o_generic_commands;
Jupiter::GenericCommandNamespace &Jupiter::g_generic_commands = o_generic_commands;

/** GenericCommand */

Jupiter::GenericCommand::GenericCommand() {
	if (&o_generic_commands != this) {
		Jupiter::GenericCommand::setNamespace(o_generic_commands);
	}
}

Jupiter::GenericCommand::~GenericCommand() {
	// Inform the parent
	if (GenericCommand::m_parent != nullptr) {
		GenericCommand::m_parent->removeCommand(*this);
	}

	// Notify plugins
	for (auto& plugin : Jupiter::plugins) {
		plugin->OnGenericCommandRemove(*this);
	}
}

bool Jupiter::GenericCommand::isNamespace() const {
	return false;
}

void Jupiter::GenericCommand::setNamespace(const Jupiter::ReadableString &in_namespace) {
	if (in_namespace.wordCount(GENERIC_COMMAND_WORD_DELIMITER_CS) == 0) {
		return; // We're already here
	}

	if (Jupiter::GenericCommand::m_parent == nullptr) {
		return; // We have no parent to start from
	}

	Jupiter::GenericCommand *command = Jupiter::GenericCommand::m_parent->getCommand(in_namespace);
	if (command != nullptr && command->isNamespace())
		Jupiter::GenericCommand::setNamespace(*static_cast<Jupiter::GenericCommandNamespace *>(command));
}

void Jupiter::GenericCommand::setNamespace(Jupiter::GenericCommandNamespace &in_namespace) {
	if (Jupiter::GenericCommand::m_parent != nullptr) { // Remove from previous parent
		Jupiter::GenericCommand::m_parent->removeCommand(*this);
	}

	// Add to new parent
	Jupiter::GenericCommand::m_parent = &in_namespace;
	in_namespace.addCommand(*this);
}

Jupiter::GenericCommandNamespace *Jupiter::GenericCommand::getNamespace() const {
	return m_parent;
}

/** GenericCommand::ResponseLine */

Jupiter::GenericCommand::ResponseLine::ResponseLine(const Jupiter::ReadableString &in_response, GenericCommand::DisplayType in_type)
	: response{ in_response },
	type{ in_type } {
}

Jupiter::GenericCommand::ResponseLine* Jupiter::GenericCommand::ResponseLine::set(const Jupiter::ReadableString &in_response, GenericCommand::DisplayType in_type) {
	response = in_response;
	type = in_type;
	return this;
}

/** GenericCommandNamespace */

Jupiter::GenericCommandNamespace::~GenericCommandNamespace() {
}

Jupiter::GenericCommand::ResponseLine* Jupiter::GenericCommandNamespace::trigger(const Jupiter::ReadableString &in_input) {
	GenericCommand* command;
	Jupiter::ReferenceString input(in_input);

	if (input.wordCount(GENERIC_COMMAND_WORD_DELIMITER_CS) == 0) { // No parameters; list commands
		return new Jupiter::GenericCommand::ResponseLine(m_help,Jupiter::GenericCommand::DisplayType::PrivateSuccess);
	}

	command = Jupiter::GenericCommandNamespace::getCommand(input.getWord(0, GENERIC_COMMAND_WORD_DELIMITER_CS));
	if (command != nullptr) {
		return command->trigger(input.gotoWord(1, GENERIC_COMMAND_WORD_DELIMITER_CS));
	}

	return new Jupiter::GenericCommand::ResponseLine(Jupiter::ReferenceString::empty, Jupiter::GenericCommand::DisplayType::PrivateError);
}

const Jupiter::ReadableString &Jupiter::GenericCommandNamespace::getHelp(const Jupiter::ReadableString &parameters) {
	static Jupiter::ReferenceString not_found = "Error: Command not found"_jrs;

	if (parameters.wordCount(GENERIC_COMMAND_WORD_DELIMITER_CS) == 0) // No parameters; list commands
	{
		if (Jupiter::GenericCommandNamespace::m_should_update_help)
			Jupiter::GenericCommandNamespace::updateHelp();

		return Jupiter::GenericCommandNamespace::m_help;
	}

	Jupiter::ReferenceString input(parameters);
	GenericCommand *command;

	// Search for command
	command = Jupiter::GenericCommandNamespace::getCommand(input.getWord(0, GENERIC_COMMAND_WORD_DELIMITER_CS));
	if (command != nullptr) {
		return command->getHelp(input.gotoWord(1, GENERIC_COMMAND_WORD_DELIMITER_CS));
	}

	// Command not found
	return not_found;
}

bool Jupiter::GenericCommandNamespace::isNamespace() const {
	return true;
}

bool Jupiter::GenericCommandNamespace::isUsing() const {
	return m_using;
}

void Jupiter::GenericCommandNamespace::setUsing(bool in_value) {
	m_using = in_value;

	if (Jupiter::GenericCommand::m_parent != nullptr)
		Jupiter::GenericCommand::m_parent->updateHelp();
}

std::vector<Jupiter::GenericCommand*> Jupiter::GenericCommandNamespace::getCommands() const {
	std::vector<Jupiter::GenericCommand*> result(m_commands.size());

	for (const auto& command : m_commands) {
		result.push_back(command);

		if (command->isNamespace() && static_cast<GenericCommandNamespace*>(command)->isUsing()) {
			// Add commands from namespace
			auto namespace_commands = static_cast<GenericCommandNamespace*>(command)->getCommands();
			for (const auto& namespace_command : namespace_commands)
				result.push_back(namespace_command);
		}
	}

	return result;
}

Jupiter::GenericCommand *Jupiter::GenericCommandNamespace::getCommand(const Jupiter::ReadableString &in_command) const {
	/** This is broken into 2 loops in order to insure that exact matches are ALWAYS prioritized over inexact matches */

	// Search commands
	for (const auto& command : m_commands) {
		if (command->matches(in_command)) {// Command found
			return command;
		}
	}

	// None found; check namespaces
	for (const auto& command : m_commands) {
		if (command->isNamespace() && static_cast<GenericCommandNamespace*>(command)->isUsing()) {
			auto result_command = static_cast<GenericCommandNamespace*>(command)->getCommand(in_command);
			if (result_command != nullptr) { // match found
				return result_command;
			}
		}
	}

	return nullptr;
}

void Jupiter::GenericCommandNamespace::addCommand(Jupiter::GenericCommand &in_command) {
	if (in_command.getNamespace() != this) {
		in_command.setNamespace(*this);
	}
	else {
		m_commands.push_back(&in_command);
		m_should_update_help = true;
	}
}

void Jupiter::GenericCommandNamespace::removeCommand(Jupiter::GenericCommand &in_command) {
	for (auto itr = m_commands.begin(); itr != m_commands.end(); ++itr) {
		if (*itr == &in_command) {
			m_commands.erase(itr);
			m_should_update_help = true;
			return;
		}
	}
}

void Jupiter::GenericCommandNamespace::removeCommand(const Jupiter::ReadableString &in_command)
{
	for (auto itr = m_commands.begin(); itr != m_commands.end(); ++itr) {
		if ((*itr)->matches(in_command)) {
			m_commands.erase(itr);
			m_should_update_help = true;
			return;
		}
	}
}

void Jupiter::GenericCommandNamespace::updateHelp() {
	m_should_update_help = false;

	std::vector<Jupiter::GenericCommand*> commands = getCommands();
	Jupiter::StringL tmp_help(commands.size() * 8);

	for (const auto& command : commands) {
		tmp_help += command->getTrigger();
		tmp_help += ' ';
	}

	m_help = tmp_help;
}
