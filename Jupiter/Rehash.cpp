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

#include "Rehash.h"
#include "DLList.h"

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
Jupiter::DLList<Jupiter::Rehashable> o_rehashables;

/** List of RehashFunction objects */
Jupiter::DLList<RehashFunction> o_rehash_functions;

Jupiter::Rehashable::Rehashable()
{
	o_rehashables.add(this);
}

Jupiter::Rehashable::Rehashable(const Jupiter::Rehashable &)
{
	o_rehashables.add(this);
}

Jupiter::Rehashable::~Rehashable()
{
	for (Jupiter::DLList<Jupiter::Rehashable>::Node *node = o_rehashables.getHead(); node != nullptr; node = node->next)
	{
		if (node->data == this)
		{
			o_rehashables.remove(node);
			break;
		}
	}
}

size_t Jupiter::rehash()
{
	size_t total_errors = 0;
	int rehash_result;
	Jupiter::DLList<Jupiter::Rehashable>::Node *node = o_rehashables.getHead();
	Jupiter::DLList<Jupiter::Rehashable>::Node *dead_node;

	while (node != nullptr)
	{
		rehash_result = node->data->OnRehash();
		if (rehash_result != 0)
		{
			++total_errors;
			if (rehash_result < 0)
			{
				dead_node = node;
				node = node->next;

				if (dead_node->data->OnBadRehash(true))
					delete o_rehashables.remove(dead_node);
				else
					o_rehashables.remove(dead_node);

				continue;
			}

			o_rehashables.remove(node)->OnBadRehash(false);
		}

		node = node->next;
	}

	return total_errors;
}

size_t Jupiter::getRehashableCount()
{
	return o_rehashables.size();
}

void Jupiter::addOnRehash(OnRehashFunctionType in_function)
{
	o_rehash_functions.add(new RehashFunction(in_function));
}

bool Jupiter::removeOnRehash(OnRehashFunctionType in_function)
{
	for (Jupiter::DLList<RehashFunction>::Node *node = o_rehash_functions.getHead(); node != nullptr; node = node->next)
	{
		if (node->data->m_function == in_function)
		{
			delete o_rehash_functions.remove(node);
			return true;
		}
	}

	return false;
}

size_t Jupiter::removeAllOnRehash()
{
	size_t result = o_rehash_functions.size();

	while (o_rehash_functions.size() != 0)
		delete o_rehash_functions.remove(size_t{ 0 });

	return result;
}
