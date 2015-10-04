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

#if !defined _TIMER_H_HEADER
#define _TIMER_H_HEADER

/**
 * @file Timer.h
 * @brief Provides an timed event system.
 */

#include "Jupiter.h"

#if defined __cplusplus

#include <ctime>
#include "Thinker.h"

namespace Jupiter
{

	/**
	* @brief Provides an interface for adding timed events.
	*/
	class JUPITER_API Timer : public Thinker
	{
	public:
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
		* @brief Constructs a timer and adds itself to the timer list.
		* Note: This assumes infinite iterations, with as little time delay as possible.
		*
		* @param function Function for the timer to call.
		* @param parameters Parameters to pass to the function.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(void(*function)(unsigned int, void *), void *parameters, bool immediate = false) : Timer(unsigned int(0), time_t(0), function, parameters, immediate) {}

		/**
		* @brief Constructs a timer and adds itself to the timer list.
		* Note: This assumes infinite iterations, with as little time delay as possible.
		*
		* @param function Function for the timer to call.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(void(*function)(unsigned int), bool immediate = false) : Timer(unsigned int(0), time_t(0), function, immediate) {}

		/**
		* @brief Constructs a timer and adds itself to the timer list.
		* Note: This assumes infinite iterations.
		*
		* @param timeDelay Delay in seconds between executions of the timed function.
		* @param function Function for the timer to call.
		* @param parameters Parameters to pass to the function.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(time_t timeDelay, void(*function)(unsigned int, void *), void *parameters, bool immediate = false) : Timer(unsigned int(0), timeDelay, function, parameters, immediate) {}
		
		/**
		* @brief Constructs a timer and adds itself to the timer list.
		* Note: This assumes infinite iterations.
		*
		* @param timeDelay Delay in seconds between executions of the timed function.
		* @param function Function for the timer to call.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(time_t timeDelay, void(*function)(unsigned int), bool immediate = false) : Timer(unsigned int(0), timeDelay, function, immediate) {}

		/**
		* @brief Constructs a timer and adds itself to the timer list.
		*
		* @param iterations Number of iterations for the timer to execute. 0 if the timer should execute indefinitely.
		* @param timeDelay Delay in seconds between executions of the timed function.
		* @param function Function for the timer to call.
		* @param parameters Parameters to pass to the function.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(unsigned int iterations, time_t timeDelay, void(*function)(unsigned int, void *), void *parameters, bool immediate = false);

		/**
		* @brief Constructs a timer and adds itself to the timer list.
		*
		* @param iterations Number of iterations for the timer to execute. 0 if the timer should execute indefinitely.
		* @param timeDelay Delay in seconds between executions of the timed function.
		* @param function Function for the timer to call.
		* @param immediate (optional) True if the function should be execute on the next check.
		*/
		Timer(unsigned int iterations, time_t timeDelay, void(*function)(unsigned int), bool immediate = false);

		/**
		* @brief Destructor for the Timer class.
		*/
		~Timer();

	/** Private data members */
	private:
		struct Data;
		Data *data_;
	};
}

extern "C"
{
#else
#include <stdbool.h>
#include <time.h>
#endif // _cplusplus

/**
* @brief Creates a timer.
*
* @param iterations Number of iterations for the timer to execute. 0 if the timer is indefinite.
* @param timeDelay Delay in seconds between executions of the timed function.
* @param immediate True if the function should be execute on the next check.
* @param function Function for the timer to call.
* @param parameters Parameters to pass to the function.
*/
JUPITER_API void Jupiter_addTimer(unsigned int iterations, time_t timeDelay, bool immediate, void(*function)(unsigned int, void *), void *parameters);

/**
* @brief Creates a timer.
*
* @param iterations Number of iterations for the timer to execute. 0 if the timer is indefinite.
* @param timeDelay Delay in seconds between executions of the timed function.
* @param immediate True if the function should be execute on the next check.
* @param function Function for the timer to call.
*/
JUPITER_API void Jupiter_addTimerNoParams(unsigned int iterations, time_t timeDelay, bool immediate, void(*function)(unsigned int));

/**
* @brief Fetches the number of active timers.
*
* @return Total number of timers active.
*/
JUPITER_API unsigned int Jupiter_totalTimers();

/**
* @brief Calls think() for every timer, and removes them if neccessary.
*
* @return Total number of timers removed.
*/
JUPITER_API unsigned int Jupiter_checkTimers();

/**
* @brief Immediately destroys all timers.
*
* @return Total number of timers removed.
*/
JUPITER_API unsigned int Jupiter_killTimers();

#if defined __cplusplus
}
#endif // __cplusplus

#endif // _TIMER_H_HEADER