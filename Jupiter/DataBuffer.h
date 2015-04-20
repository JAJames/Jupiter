/**
 * Copyright (C) 2015 Justin James.
 *
 * This license must be preserved.
 * Any applications, libraries, or code which make any use of any
 * component of this program must not be commercial, unless explicit
 * permission is granted from the original author. The use of this
 * program for non-profit purposes is permitted.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In the event that this license restricts you from making desired use of this program, contact the original author.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#if !defined _DATABUFFER_H_HEADER
#define _DATABUFFER_H_HEADER

/**
 * @file DataBuffer.h
 * @brief Defines a simple data buffer structure.
 */

#include <cstdint>
#include "Jupiter.h"

namespace Jupiter
{
	/**
	* @brief Provides a data buffer implementation intended for files and sockets.
	* DataBuffer behaves as a FIFO single-directional queue.
	*/
	class JUPITER_API DataBuffer
	{
	public:

		/**
		* @brief Generates an object based on an input head position; the head
		* will be moved if the input type can be interpreted.
		*
		* @return Generated object.
		*/
		template<typename T> static T interpret_data(uint8_t *&head);
		template<template<typename> class T, typename Y> static T<Y> interpret_data(uint8_t *&head);
		template<template<typename, typename> class T, typename X, typename Y> static T<X, Y> interpret_data(uint8_t *&head);
		template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> static T<X, Y, Z> interpret_data(uint8_t *&head);

		/**
		* @brief Copies data from the front of the buffer, without removing it.
		*
		* @param T Data type
		*
		* @return Sum of all the elements in the string
		*/
		template<typename T> T peek() const;
		template<template<typename> class T, typename Y> T<Y> peek() const;
		template<template<typename, typename> class T, typename X, typename Y> T<X, Y> peek() const;
		template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> T<X, Y, Z> peek() const;

		template<typename T> void peek(T &out) const;
		template<template<typename> class T, typename Y> void peek(T<Y> &out) const;
		template<template<typename, typename> class T, typename X, typename Y> void peek(T<X, Y> &out) const;
		template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> void peek(T<X, Y, Z> &out) const;

		/**
		* @brief Copies data from the front of the buffer, without removing it.
		* Note: Elements will be in the opposite order than concurrent calls
		* to peek() would produce.
		*
		* @param T Data type
		* @param size Number of elements to pop
		*
		* @return Sum of all the elements in the string
		*/
		template<typename T> T *peek(size_t size) const;

		/**
		* @brief Copies data from the front of the buffer and removes it.
		*
		* @param T Data type
		*
		* @return Sum of all the elements in the string
		*/
		template<typename T> T pop();
		template<template<typename> class T, typename Y> T<Y> pop();
		template<template<typename, typename> class T, typename X, typename Y> T<X, Y> pop();
		template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> T<X, Y, Z> pop();

		template<typename T> void pop(T &out);
		template<template<typename> class T, typename Y> void pop(T<Y> &out);
		template<template<typename, typename> class T, typename X, typename Y> void pop(T<X, Y> &out);
		template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> void pop(T<X, Y, Z> &out);

		/**
		* @brief Copies data from the front of the buffer and removes it.
		* Note: Elements will be in the opposite order than concurrent calls
		* to pop() would produce.
		*
		* @param T Data type
		* @param size Number of elements to pop
		*
		* @return Sum of all the elements in the string
		*/
		template<typename T> T *pop(size_t size);

		/**
		* @brief Adds data to the front of the buffer.
		*
		* @param T Data type
		* @param data Data to add to the buffer
		*/
		template<typename T> void push(const T *data);
		template<typename T> void push(const T &data);

		// Specialization: Jupiter::ReadableString + Sub classes
		template<template<typename> class T, typename Y> void push(const T<Y> *data);
		template<template<typename> class T, typename Y> void push(const T<Y> &data);

		// SPECIALIZATION TODO: std::deque - std::list - std::forward_list

		// SPECIALIZATION: std::array
		template<template<typename, size_t> class T, typename X, size_t Y> void push(const T<X, Y> *data);
		template<template<typename, size_t> class T, typename X, size_t Y> void push(const T<X, Y> &data);

		// SPECIALIZATION: std::vector
		template<template<typename, typename> class T, typename X, typename Y> void push(const T<X, Y> *data);
		template<template<typename, typename> class T, typename X, typename Y> void push(const T<X, Y> &data);

		// SPECIALIZATION: std::basic_string (std::string / std::wstring / etc)
		template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> void push(const T<X, Y, Z> *data);
		template<template<typename, typename, typename> class T, typename X, typename Y, typename Z> void push(const T<X, Y, Z> &data);

		/**
		* @brief Adds data to the end of the buffer.
		*
		* @param data Data to add to the buffer
		* @param size Number of octets in data to add
		*/
		void push(const uint8_t *data, size_t size);

		/**
		* @brief Shrinks the buffer to the smallest possible size.
		*
		* @return New size of the buffer in octets.
		*/
		size_t shrink();

		/**
		* @brief Expands the size of the internal buffer by a set number of octets.
		*
		* @param size Number of octets to 
		* @return New size of the buffer in octets.
		*/
		size_t expandBuffer(size_t size);

		/**
		* @brief Guarantees availability for a specified number of octets.
		*
		* @param size Number of octets to guarantee.
		* @return New size of the buffer in octets.
		*/
		void secure(size_t size);

		/**
		* @brief Sets the size of the internal buffer.
		* Note: If input 'size' is less than size(), data is removed.
		*
		* @param size Size to set the buffer to.
		* @return New size of the buffer in octets.
		*/
		void setBufferSize(size_t size);

		/**
		* @brief Fetches the size of the data in octets.
		*
		* @return Size of the data in octets.
		*/
		size_t size() const;

		/**
		* @brief Fetches the size of the buffer in octets.
		*
		* @return Size of the buffer in octets.
		*/
		size_t getBufferSize() const;

		/**
		* @brief Fetches the head of the buffer
		*
		* @return Buffer head
		*/
		uint8_t *getHead() const;

		/**
		* @brief Empties the buffer of all elements.
		* Note: The internal memory buffer is not freed or reallocated.
		*/
		void empty();

		DataBuffer();
		DataBuffer(size_t size);
		~DataBuffer();

	private:
		uint8_t *base;
		uint8_t *head;
		uint8_t *end;
		size_t bufferSize;
	};
}

#include "DataBuffer_Imp.h"

#endif // _DATABUFFER_H_HEADER