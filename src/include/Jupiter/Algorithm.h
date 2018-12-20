/**
 * Copyright (C) 2017 Jessica James.
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

#if !defined _ALGORITHM_H_HEADER
#define _ALGORITHM_H_HEADER

/**
 * @file Algorithm.h
 * @brief Provides various common algorithm implementations.
 */

namespace Jupiter
{
	namespace Algorithm
	{
		/**
		* @brief Sorts an array of data in-place using merge sort across multiple threads
		*
		* @param in_data Data to sort
		* @param in_data_length Length of the array of data to sort
		*/
		template<typename T> void merge_sort(T *in_data, size_t in_data_length);

		/**
		* @brief Sorts an array of data in-place using merge sort across multiple threads
		*
		* @param in_data Data to sort
		* @param in_data_length Length of the array of data to sort
		*/
		template<typename T> void merge_sort_parallel(T *in_data, size_t in_data_length);

		/**
		* @brief Sorts an array of data in-place using merge sort across multiple threads
		*
		* @param in_data Data to sort
		* @param in_data_length Length of the array of data to sort
		* @param in_max_threads Maximum number of threads to use
		* @param in_use_max_threads True to force the number of threads to in_max_threads
		*/
		template<typename T> void merge_sort_parallel(T *in_data, size_t in_data_length, unsigned int in_max_threads, bool in_force_max_threads = false);

		/**
		* @brief Sorts an array of data in-place using merge sort
		*
		* @param in_data Data to sort
		* @param in_data_length Length of the array of data to sort
		*/
		template<typename T> void merge_sort_serial(T *in_data, size_t in_data_length);

		/**
		* @brief Sorts an array of data in-place using merge sort across the specified number of threads
		*
		* @param in_source Data to sort
		* @param in_length Length of the array of data to sort
		* @param out_buffer Working array for sorting; must be the same size as in_source
		* @param unused_threads Number of additional threads to spawn (not including the thread calling the function)
		*/
		template<typename T> void merge_sort_direct(T *in_source, size_t in_length, T *out_buffer, unsigned int unused_threads);
	}
}

/**
* IMPLEMENTATION:
*	Merge Sort
*/

template<typename T> void Jupiter::Algorithm::merge_sort(T *in_data, size_t in_data_length)
{
	merge_sort_parallel<T>(in_data, in_data_length);
}

template<typename T> void Jupiter::Algorithm::merge_sort_parallel(T *in_data, size_t in_data_length)
{
	merge_sort_parallel<T>(in_data, in_data_length, std::thread::hardware_concurrency());
}

template<typename T> void Jupiter::Algorithm::merge_sort_parallel(T *in_data, size_t in_data_length, unsigned int in_max_threads, bool in_force_max_threads)
{
	constexpr size_t MINIMUM_ELEMENTS_PER_THREAD = 0x3FFF;

	unsigned int thread_count = static_cast<unsigned int>(in_data_length) / MINIMUM_ELEMENTS_PER_THREAD;

	if (thread_count > in_max_threads || in_force_max_threads)
		thread_count = in_max_threads;

	if (thread_count <= 1)
	{
		merge_sort_serial<T>(in_data, in_data_length);
		return;
	}

	T *buffer = new T[in_data_length];

	// copy base into buffer
	for (size_t index = 0; index != in_data_length; ++index)
		buffer[index] = in_data[index];

	// mergesort into buffer
	merge_sort_direct<T>(in_data, in_data_length, buffer, thread_count - 1);

	// mergesort is complete; free buffer
	delete[] buffer;
}

template<typename T> void Jupiter::Algorithm::merge_sort_serial(T *in_data, size_t in_data_length)
{
	if (in_data_length <= 1)
		return;

	T *buffer = new T[in_data_length];

	// copy base into buffer
	for (size_t index = 0; index != in_data_length; ++index)
		buffer[index] = in_data[index];

	// mergesort into buffer
	merge_sort_direct<T>(in_data, in_data_length, buffer, 0);

	// mergesort is complete; free buffer
	delete[] buffer;
}

/** Internals */

template<typename T> void Jupiter::Algorithm::merge_sort_direct(T *in_source, size_t in_length, T *out_buffer, unsigned int unused_threads)
{
	if (in_length <= 1) // Nothing to sort
		return;

	/** Split */

	T *left_buffer = out_buffer;
	size_t left_length = in_length / 2;

	T *right_buffer = out_buffer + left_length;
	size_t right_length = in_length - left_length;

	if (unused_threads == 0) // serial
	{
		// sort left
		merge_sort_direct<T>(left_buffer, left_length, in_source, 0);

		// sort right
		merge_sort_direct<T>(right_buffer, right_length, in_source + left_length, 0);
	}
	else // parallel
	{
		--unused_threads;

		unsigned int left_thread_count = unused_threads / 2;

		// sort left
		std::thread left_sort_thread(merge_sort_direct<T>, left_buffer, left_length, in_source, left_thread_count);

		// sort right
		merge_sort_direct<T>(right_buffer, right_length, in_source + left_length, unused_threads - left_thread_count);

		left_sort_thread.join();
	}

	// merge left & right
	while (true)
	{
		if (*right_buffer < *left_buffer)
		{
			*in_source = *right_buffer;

			++in_source;
			++right_buffer;

			if (--right_length == 0)
			{
				// Only values on the left side remain
				while (left_length != 0)
				{
					*in_source = *left_buffer;

					++in_source;
					++left_buffer;
					--left_length;
				}

				return;
			}
		}
		else
		{
			*in_source = *left_buffer;

			++in_source;
			++left_buffer;

			if (--left_length == 0)
			{
				// Only values on the right side remain
				while (right_length != 0)
				{
					*in_source = *right_buffer;

					++in_source;
					++right_buffer;
					--right_length;
				}

				return;
			}
		}
	}
}

#endif // _ALGORITHM_H_HEADER
