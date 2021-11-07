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

#include <cstring>
#include "Command.h"
#include "String.hpp"

struct Jupiter::Command::Data { // TODO: remove pimpl
	std::vector<Jupiter::StringS> triggers;
};

Jupiter::Command::Command() {
	m_data = new Data();
}

Jupiter::Command::Command(const Command &command) {
	m_data = new Data();
	//for (size_t i = 0; i != m_data->triggers.size(); i++) // triggers.size() would always be 0? this code does nothing?
	//	m_data->triggers.add(new Jupiter::StringS(*command.m_data->triggers.get(i)));
}

Jupiter::Command::~Command() {
	delete m_data;
}

// Command Functions

void Jupiter::Command::addTrigger(const Jupiter::ReadableString &trigger) {
	m_data->triggers.emplace_back(trigger);
}

const Jupiter::ReadableString &Jupiter::Command::getTrigger(size_t index) const {
	return m_data->triggers[index];
}

size_t Jupiter::Command::getTriggerCount() const {
	return m_data->triggers.size();
}

bool Jupiter::Command::matches(const Jupiter::ReadableString &in_trigger) const {
	for (const auto& trigger : m_data->triggers)
		if (trigger.equalsi(in_trigger)) return true;
	return false;
}