/**
 * Copyright (C) 2014-2017 Jessica James.
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

#include <list>
#include "Rehash.h"

/**
* Considerations:
*	It may be worth-while making rehashables aware of which node they're in, or integrating them into their own node?
*/

/** Wrapper class to add rehashable functions. */
class RehashFunction : public Jupiter::Rehashable
{
public:
	Jupiter::OnRehashFunctionType m_function;
	
	int OnRehash()
	{
		return m_function();
	}

	bool OnBadRehash(bool in_removed)
	{
		return in_removed;
	}

	RehashFunction(Jupiter::OnRehashFunctionType in_function)
	{
		m_function = in_function;
	}
};

/** List of Rehashable objects */
std::list<Jupiter::Rehashable *> o_rehashables;

/** List of RehashFunction objects */
std::list<RehashFunction *> o_rehash_functions;

Jupiter::Rehashable::Rehashable()
{
	o_rehashables.push_back(this);
}

Jupiter::Rehashable::Rehashable(const Jupiter::Rehashable &)
{
	o_rehashables.push_back(this);
}

Jupiter::Rehashable::~Rehashable()
{
	for (auto node = o_rehashables.begin(); node != o_rehashables.end(); ++node)
	{
		if (*node == this)
		{
			o_rehashables.erase(node);
			break;
		}
	}
}

size_t Jupiter::rehash()
{
	size_t total_errors = 0;
	int rehash_result;
	auto node = o_rehashables.begin();

	while (node != o_rehashables.end())
	{
		rehash_result = (*node)->OnRehash();
		if (rehash_result != 0)
		{
			++total_errors;
			if (rehash_result < 0)
			{
				auto dead_node = node;
				++node;

				if ((*dead_node)->OnBadRehash(true))
					delete *dead_node;

				o_rehashables.erase(dead_node);

				continue;
			}

			(*node)->OnBadRehash(false);
		}

		++node;
	}

	return total_errors;
}

size_t Jupiter::getRehashableCount()
{
	return o_rehashables.size();
}

void Jupiter::addOnRehash(OnRehashFunctionType in_function)
{
	o_rehash_functions.push_back(new RehashFunction(in_function));
}

bool Jupiter::removeOnRehash(OnRehashFunctionType in_function)
{
	for (auto node = o_rehash_functions.begin(); node != o_rehash_functions.end(); ++node)
	{
		if ((*node)->m_function == in_function)
		{
			delete *node;
			o_rehash_functions.erase(node);
			return true;
		}
	}

	return false;
}

size_t Jupiter::removeAllOnRehash()
{
	size_t result = o_rehash_functions.size();

	while (o_rehash_functions.size() != 0)
	{
		delete o_rehash_functions.front();
		o_rehash_functions.pop_front();
	}

	return result;
}
