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

#if !defined _TIMER_H_HEADER
#define _TIMER_H_HEADER

/**
 * @file Timer.h
 * @brief Provides a timed event system.
 */

#include <chrono>
#include "Jupiter.h"
#include "Thinker.h"

/** DLL Linkage Nagging */
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace Jupiter
{
	/**
	* @brief Provides an interface for adding timed events.
	*/
	class JUPITER_API Timer : public Thinker
	{
	public:
		typedef void(*FunctionType)(unsigned int, void *);

		/**
		* @brief Checks if the timed function should be called, and calls it if neccessary.
		* Note: This is primarily used internally by checkTimers().
		*
		* @return 0 if the function still has more iterations to call, a number otherwise.
		*/
		int think();

		/**
		* @brief Removes the timer from the timer list, without destroying it.
		* Note: You MUST delete the timer later yourself after calling this.
		*
		* @return True if the timer was removed from the list, false otherwise.
		*/
		bool removeFromList();

		/**
		* @brief Immediately destroys the timer and removes it from the timer list.
		*
		* @return True if the timer was removed from the list, false otherwise.
		*/
		bool kill();

		/**
		* @brief Fetches the number of active timers.
		*
		* @return Total number of timers active.
		*/
		static size_t total();

		/**
		* @brief Calls think() for every timer, and removes them if neccessary.
		*
		* @return Total number of timers removed.
		*/
		static size_t check();

		/**
		* @brief Immediately destroys all timers.
		*
		* @return Total number of timers removed.
		*/
		static size_t killAll();

		/**
		* @brief Constructs a timer and adds itself to the timer list.
		* Note: This assumes infinite iterations, with as little time delay as possible.
		*
		* @param function Function for the timer to call.
		* @param parameters Parameters to pass to the function.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(FunctionType in_function, void *in_parameters, bool in_immediate = false) : Timer(0U, std::chrono::milliseconds(0), in_function, in_parameters, in_immediate) {}

		/**
		* @brief Constructs a timer and adds itself to the timer list.
		* Note: This assumes infinite iterations, with as little time delay as possible.
		*
		* @param function Function for the timer to call.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(FunctionType in_function, bool in_immediate = false) : Timer(0U, std::chrono::milliseconds(0), in_function, in_immediate) {}

		/**
		* @brief Constructs a timer and adds itself to the timer list.
		* Note: This assumes infinite iterations.
		*
		* @param timeDelay Delay in seconds between executions of the timed function.
		* @param function Function for the timer to call.
		* @param parameters Parameters to pass to the function.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(std::chrono::milliseconds in_delay, FunctionType in_function, void *in_parameters, bool in_immediate = false) : Timer(0U, in_delay, in_function, in_parameters, in_immediate) {}

		/**
		* @brief Constructs a timer and adds itself to the timer list.
		* Note: This assumes infinite iterations.
		*
		* @param timeDelay Delay in seconds between executions of the timed function.
		* @param function Function for the timer to call.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(std::chrono::milliseconds in_delay, FunctionType in_function, bool in_immediate = false) : Timer(0U, in_delay, in_function, in_immediate) {}

		/**
		* @brief Constructs a timer and adds itself to the timer list.
		*
		* @param iterations Number of iterations for the timer to execute. 0 if the timer should execute indefinitely.
		* @param timeDelay Delay in seconds between executions of the timed function.
		* @param function Function for the timer to call.
		* @param parameters Parameters to pass to the function.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(unsigned int in_iterations, std::chrono::milliseconds in_delay, FunctionType in_function, void *in_parameters, bool in_immediate = false);

		/**
		* @brief Constructs a timer and adds itself to the timer list.
		*
		* @param iterations Number of iterations for the timer to execute. 0 if the timer should execute indefinitely.
		* @param timeDelay Delay in seconds between executions of the timed function.
		* @param function Function for the timer to call.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(unsigned int in_iterations, std::chrono::milliseconds in_delay, FunctionType in_function, bool in_immediate = false);

	private:
		/** Private data members */
		std::chrono::steady_clock::time_point m_next_call;
		std::chrono::milliseconds m_delay;
		unsigned int m_iterations;

		FunctionType m_function;
		void *m_parameters;
	};
}

/** Re-enable warnings */
#if defined _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // _TIMER_H_HEADER