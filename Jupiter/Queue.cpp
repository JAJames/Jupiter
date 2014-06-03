#include "Queue.h"

struct Jupiter::Queue::Data
{
	Data *next;
	void *data;
};

Jupiter::Queue::Queue()
{
	Jupiter::Queue::end = new Jupiter::Queue::Data();
	Jupiter::Queue::end->next = nullptr;
	Jupiter::Queue::head = new Jupiter::Queue::Data();
	Jupiter::Queue::head->next = Jupiter::Queue::end;
	Jupiter::Queue::head->data = nullptr;
	Jupiter::Queue::length = 0;
}

Jupiter::Queue::~Queue()
{
	Jupiter::Queue::Data *p;
	Jupiter::Queue::Data *c = head;
	do
	{
		p = c;
		c = c->next;
		delete p;
	}
	while (c != nullptr);
}

void Jupiter::Queue::enqueue(void *data)
{
	Jupiter::Queue::Data *nEnd = new Jupiter::Queue::Data();
	nEnd->next = nullptr;
	end->data = data;
	end->next = nEnd;
	end = nEnd;
	Jupiter::Queue::length++;
}

void *Jupiter::Queue::dequeue()
{
	if (Jupiter::Queue::length > 0)
	{
		Jupiter::Queue::Data *tmp = Jupiter::Queue::head->next;
		Jupiter::Queue::head->next = tmp->next;
		void *data = tmp->data;
		delete tmp;
		Jupiter::Queue::length--;
		return data;
	}
	return nullptr;
}

size_t Jupiter::Queue::size() const
{
	return Jupiter::Queue::length;
}