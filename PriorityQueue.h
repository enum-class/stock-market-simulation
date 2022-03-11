#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_

#include <vector>
#include <array>
#include <utility>
#include <unordered_map>


template < typename QueueItem, typename Id, size_t CAPACITY = 102400, typename Less = std::less<QueueItem>, typename Greater = std::greater<QueueItem> >
class PriorityQueue
{
public:
	inline PriorityQueue() noexcept;

	/// Inserts items in priority queue and index map
	inline void insert(const QueueItem& item) noexcept;

	/// Removes items in priority queue and index map
	inline void remove(const QueueItem& item) noexcept;

	/// Updates items in priority queue and index map
	inline void update(const QueueItem& item) noexcept;

	/// Returns number of item exist in data structure
	inline size_t get_orders_count() const noexcept;
	
	/// Returns K top item in heap data structure
	inline std::vector<QueueItem> get_top_items(size_t k) const noexcept;
	
	/// Filter data based on specific criteria
	template <typename Value, typename Match, typename Compare>
	inline std::pair<QueueItem, bool> filter(const Value& value, Match match, Compare compare) const noexcept;

private:
	inline static size_t getNearestCount(size_t k) noexcept;

	inline void swap(size_t a, size_t b) noexcept;

	inline static size_t heap_parent(size_t n) noexcept;

	inline static size_t heap_left_son(size_t n) noexcept;

	inline static size_t heap_right_son(size_t n) noexcept;

	inline void heapify(size_t position) noexcept;

	std::unordered_map<Id, size_t> table;
	std::array<QueueItem, CAPACITY> heap;

	size_t size;
	Less less;
	Greater greater;
};

#include "PriorityQueue-inl.h"

#endif
