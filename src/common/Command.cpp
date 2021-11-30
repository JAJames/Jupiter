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
#include "jessilib/unicode.hpp"
#include "Command.h"

// Command Functions

void Jupiter::Command::addTrigger(std::string_view trigger) {
	m_triggers.emplace_back(trigger);
}

std::string_view Jupiter::Command::getTrigger(size_t index) const {
	return m_triggers[index];
}

size_t Jupiter::Command::getTriggerCount() const {
	return m_triggers.size();
}

bool Jupiter::Command::matches(std::string_view in_trigger) const {
	for (const auto& trigger : m_triggers) {
		if (jessilib::equalsi(trigger, in_trigger)) {
			return true;
		}
	}
	return false;
}