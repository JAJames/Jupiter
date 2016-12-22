/**
 * Copyright (C) 2014-2016 Jessica James.
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

#include "Timer.h"
#include "DLList.h"

Jupiter::DLList<Jupiter::Timer> o_timers;

/** Deallocates timers when the library is unloaded. */
struct TimerKiller
{
	~TimerKiller();
} timerKiller;

TimerKiller::~TimerKiller()
{
	Jupiter::Timer::killAll();
}

Jupiter::Timer::Timer(unsigned int in_iterations, std::chrono::milliseconds in_delay, FunctionType in_function, void *in_parameters, bool in_immediate)
{
	m_function = in_function;
	m_parameters = in_parameters;
	m_iterations = in_iterations;
	m_delay = in_delay;
	m_next_call = std::chrono::steady_clock::now();

	if (in_immediate == false)
		m_next_call += m_delay;

	o_timers.add(this);
}

Jupiter::Timer::Timer(unsigned int in_iterations, std::chrono::milliseconds in_delay, FunctionType in_function, bool in_immediate)
{
	m_function = in_function;
	m_parameters = nullptr;
	m_iterations = in_iterations;
	m_delay = in_delay;
	
	if (in_immediate == false)
		m_next_call += m_delay;

	o_timers.add(this);
}

int Jupiter::Timer::think()
{
	if (m_next_call <= std::chrono::steady_clock::now())
	{
		int killMe = 0;

		if (m_iterations != 0 && --m_iterations == 0)
			killMe = 1;

		m_function(m_iterations, m_parameters);
		m_next_call = m_delay + std::chrono::steady_clock::now();

		return killMe;
	}

	return 0;
}

bool Jupiter::Timer::removeFromList()
{
	for (Jupiter::DLList<Jupiter::Timer>::Node *node = o_timers.getHead(); node != nullptr; node = node->next)
	{
		if (node->data == this)
		{
			o_timers.remove(node);
			return true;
		}
	}

	return false;
}

bool Jupiter::Timer::kill()
{
	if (this->removeFromList())
	{
		delete this;
		return true;
	}

	delete this;
	return false;
}

size_t Jupiter::Timer::total()
{
	return o_timers.size();
}

size_t Jupiter::Timer::check()
{
	size_t result = 0;
	Jupiter::Timer *timer;
	Jupiter::DLList<Jupiter::Timer>::Node *dead_node;
	Jupiter::DLList<Jupiter::Timer>::Node *node = o_timers.getHead();

	while (node != nullptr)
	{
		timer = node->data;
		if (timer->think() != 0)
		{
			dead_node = node;
			node = node->next;
			delete o_timers.remove(dead_node);
			++result;
		}
		else
			node = node->next;
	}

	return result;
}

size_t Jupiter::Timer::killAll()
{
	size_t result = o_timers.size();

	while (o_timers.size() != 0)
		delete o_timers.remove(size_t{ 0 });

	return result;
}