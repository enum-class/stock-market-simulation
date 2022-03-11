#ifndef PRIORITY_QUEUE_INL_H_
#define PRIORITY_QUEUE_INL_H_

#ifndef PRIORITY_QUEUE_H_
#error "PriorityQueue-inl.h" should be included only in "PriorityQueue.h" file
#endif

#include <algorithm>
#include <math.h>

#include "Logger.h"

using namespace std;

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::PriorityQueue() noexcept
: size(0)
{
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
void PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::insert(const QueueItem& item) noexcept
{
	auto search = table.find(item.get_id());
	if (search != table.end())
	{
		Logger::warning("PriorityQueue::insert(): item already exist.");
		return;
	}

	heap[size] = item;
	table[item.get_id()] = size;
	heapify(size);
	++size;
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
void PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::remove(const QueueItem& item) noexcept
{
	auto search = table.find(item.get_id());
	if (search == table.end())
	{
		Logger::warning("PriorityQueue::remove(): item not found.");
		return;
	}

	size_t position = search->second;
	const QueueItem& old = heap[position];

	uint32_t old_position = position;
	uint32_t new_position = size - 1;

	swap(old_position, new_position);
	--size;

	table.erase(search);
	heapify(old_position);
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
void PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::update(const QueueItem& item) noexcept
{
	auto search = table.find(item.get_id());
	if (search == table.end())
	{
		Logger::warning("PriorityQueue::update(): item not found.");
		return;
	}

	size_t position = search->second;
	const QueueItem& old = heap[position];

	heap[position] = item;
	heapify(position);
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
size_t PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::get_orders_count() const noexcept
{
	return size;
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
vector<QueueItem> PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::get_top_items(size_t k) const noexcept
{
	k = min(k, size);
	if (k == 0)
		return vector<QueueItem>(0);

	size_t n = getNearestCount(k);
	size_t start_index = pow(2, n) - 1;
	size_t end_index = pow(2, n + 1) - 1;

	if (k == end_index)
	{
		vector<QueueItem> biggest(heap.begin(), heap.begin() + end_index);
		return biggest;
	}
	else
	{
		vector<QueueItem> biggest(heap.begin(), heap.begin() + start_index);

		vector<QueueItem> part(heap.begin() + start_index, heap.begin() + end_index);
		sort(part.begin(), part.end());

		biggest.insert(biggest.end(), part.begin(), part.begin() + k - start_index);
		return biggest;
	}
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
template <typename Value, typename Match, typename Compare>
pair<QueueItem, bool> PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::filter(const Value& value, Match match, Compare compare) const noexcept
{
	QueueItem best;
	bool is_match = false;

	for (int i=0; i < size; i++)
	{
		if (match(heap[i], value))
		{
			is_match = true;
			if (compare(best, heap[i]))
				best = heap[i];
		}
	}

	return make_pair(best, is_match);
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
size_t PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::getNearestCount(size_t k) noexcept
{
	return size_t(log(k) / log(2));
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
void PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::swap(size_t a, size_t b) noexcept
{
	QueueItem tmp = heap[a];
	heap[a] = heap[b];
	heap[b] = tmp;

	size_t tmp_position = table[heap[a].get_id()];
	table[heap[a].get_id()] = table[heap[a].get_id()];
	table[heap[a].get_id()] = tmp_position; 
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
size_t PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::heap_parent(size_t n) noexcept
{
	return (n - 1) / 2;
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
size_t PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::heap_left_son(size_t n) noexcept
{
	return n * 2 + 1;
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
size_t PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::heap_right_son(size_t n) noexcept
{
	return n * 2 + 2;
}

template <typename QueueItem, typename Id, size_t CAPACITY, typename Less, typename Greater>
void PriorityQueue<QueueItem, Id, CAPACITY, Less, Greater>::heapify(size_t position) noexcept
{
	size_t i = position;

	// Moves the node upwards
	for (; i > 0 && heap[i] < heap[heap_parent(i)]; i = heap_parent(i))
		swap(i, heap_parent(i));

	/// Moves the node downwards
	while (i < size && heap_left_son(i) < size)
	{
		const uint32_t left = heap_left_son(i);
		const uint32_t right = heap_right_son(i);
		size_t b = left;
		if (right < size && less(heap[right], heap[left]))
			b = right;
		if (b >= size || !greater(heap[i], heap[b]))
			break;
		swap(i, b);
		i = b;
	}
}

#endif
