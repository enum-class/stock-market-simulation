#include <gtest/gtest.h>
#include <iostream>

#include "../PriorityQueue.h"

struct Item
{
	int data;
	int id;

	int get_id() const
	{
		return id;
	}

	bool operator==(const Item& p) const
	{
		return id == p.id;
	}

	bool operator<(const Item& p) const
	{
		return data > p.data;
	}

	bool operator>(const Item& p) const
	{
		return data < p.data;
	}
};

TEST(PriorityQueue, insert)
{

	PriorityQueue<Item, int> queue;
	queue.insert({5, 10});

	ASSERT_EQ(queue.get_orders_count(), 1);
}

TEST(PriorityQueue, remove)
{
	PriorityQueue<Item, int> queue;
	queue.insert({5, 10});
	queue.remove({5, 10});

	ASSERT_EQ(queue.get_orders_count(), 0);
}

TEST(PriorityQueue, update)
{
	PriorityQueue<Item, int> queue;
	queue.insert({5, 10});
	queue.update({5, 20});

	ASSERT_EQ(queue.get_orders_count(), 1);
}
