/**
 * Copyright (C) 2014-2015 Justin James.
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
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include "Timer.h"
#include "DLList.h"

Jupiter::DLList<Jupiter::Timer> timers;

/** Deallocates timers when the library is unloaded. */
struct TimerKiller
{
	~TimerKiller();
} timerKiller;

TimerKiller::~TimerKiller()
{
	Jupiter_killTimers();
}

struct Jupiter::Timer::Data
{
	void (*function)(unsigned int, void *);
	void (*functionNoParams)(unsigned int);
	void *parameters;
	time_t nextCall;
	time_t delay;
	int iterations;
};

Jupiter::Timer::Timer(unsigned int iterations, time_t timeDelay, void(*function)(unsigned int, void *), void *parameters, bool immediate)
{
	Jupiter::Timer::data_ = new Jupiter::Timer::Data();
	Jupiter::Timer::data_->function = function;
	Jupiter::Timer::data_->functionNoParams = nullptr;
	Jupiter::Timer::data_->parameters = parameters;
	Jupiter::Timer::data_->iterations = iterations;
	Jupiter::Timer::data_->delay = timeDelay;
	Jupiter::Timer::data_->nextCall = immediate ? time(0) : time(0) + timeDelay;
	timers.add(this);
}

Jupiter::Timer::Timer(unsigned int iterations, time_t timeDelay, void(*function)(unsigned int), bool immediate)
{
	Jupiter::Timer::data_ = new Jupiter::Timer::Data();
	Jupiter::Timer::data_->function = nullptr;
	Jupiter::Timer::data_->functionNoParams = function;
	Jupiter::Timer::data_->parameters = nullptr;
	Jupiter::Timer::data_->iterations = iterations;
	Jupiter::Timer::data_->delay = timeDelay;
	Jupiter::Timer::data_->nextCall = immediate ? time(0) : time(0) + timeDelay;
	timers.add(this);
}

Jupiter::Timer::~Timer()
{
	delete Jupiter::Timer::data_;
}

int Jupiter::Timer::think()
{
	if (Jupiter::Timer::data_->nextCall <= time(0))
	{
		int killMe = 0;

		if (Jupiter::Timer::data_->iterations != 0 && --Jupiter::Timer::data_->iterations == 0) killMe = 1;
		if (Jupiter::Timer::data_->function != nullptr) Jupiter::Timer::data_->function(Jupiter::Timer::data_->iterations, Jupiter::Timer::data_->parameters);
		else Jupiter::Timer::data_->functionNoParams(Jupiter::Timer::data_->iterations);

		Jupiter::Timer::data_->nextCall = Jupiter::Timer::data_->delay + time(0);

		return killMe;
	}

	return 0;
}

bool Jupiter::Timer::removeFromList()
{
	if (timers.size() == 0) return false;
	for (Jupiter::DLList<Jupiter::Timer>::Node *n = timers.getNode(0); n != nullptr; n = n->next)
	{
		if (n->data == this)
		{
			timers.remove(n);
			return true;
		}
	}
	return false;
}

bool Jupiter::Timer::kill()
{
	if (Jupiter::Timer::removeFromList())
	{
		delete this;
		return true;
	}
	delete this;
	return false;
}

extern "C" void Jupiter_addTimer(unsigned int iterations, time_t timeDelay, bool immediate, void(*function)(unsigned int, void *), void *parameters)
{
	new Jupiter::Timer(iterations, timeDelay, function, parameters, immediate);
}

extern "C" void Jupiter_addTimerNoParams(unsigned int iterations, time_t timeDelay, bool immediate, void(*function)(unsigned int))
{
	new Jupiter::Timer(iterations, timeDelay, function, immediate);
}

extern "C" unsigned int Jupiter_totalTimers()
{
	return timers.size();
}

extern "C" unsigned int Jupiter_checkTimers()
{
	unsigned int r = 0;
	Jupiter::Timer *t;
	Jupiter::DLList<Jupiter::Timer>::Node *o;
	Jupiter::DLList<Jupiter::Timer>::Node *n = timers.size() == 0 ? nullptr : timers.getNode(0);
	while (n != nullptr)
	{
		t = n->data;
		if (t->think() != 0)
		{
			o = n;
			n = n->next;
			delete timers.remove(o);
			r++;
		}
		else n = n->next;
	}
	return r;
}

extern "C" unsigned int Jupiter_killTimers()
{
	unsigned int r = timers.size();
	while (timers.size() != 0)
		delete timers.remove(0U);
	return r;
}