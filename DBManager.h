#ifndef DB_MANAGER_H_
#define DB_MANAGER_H_

#include <array>
#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>

#include "PriorityQueue.h"

/// DBManager manage parsing transactions and connection to the data structure that saves information.
class DBManager
{
	static constexpr size_t COMMAND_PART_COUNT = 7;

	template<int N>
	using StringArray = std::array<std::string, N>;

	template<int N>
	using ConstStringArray = const std::array<std::string, N>;

	/// Enum class represent transactions instruction type.
	enum class Instruction : uint8_t
	{
		INSERT,
		CANCEL,
		AMEND,
		UNKNOW
	};

	/// Enum class represent transactions side. That means it is buy or sell.
	enum class Side : uint8_t
	{
		BUY,
		SELL,
		UNKNOW
	};

	/// Struct that contains order information
	struct Order
	{
		uint32_t id;
		std::string time;
		uint32_t volume;
		double price;
		
		uint32_t get_id() const noexcept
		{
			return id;
		}

		bool operator==(const Order& p) const noexcept
		{
			return id == p.id;
		}

		bool operator<(const Order& p) const noexcept
		{
			return volume > p.volume;
		}

		bool operator>(const Order& p) const noexcept
		{
			return volume < p.volume;
		}
	};

	/**
	 * Staruct that represent as key in information table.
	 * It contains Symbol and side of transaction means buy or sell.
	 */
	struct Key
	{
		std::string symbol;
		Side side;

		bool operator==(const Key& p) const noexcept
		{
			return symbol == p.symbol && side == p.side;
		}
	};

	/// Hash functor for key of table of information
	struct hash_fn
	{
		size_t operator() (const Key& node) const noexcept
		{
			size_t h1 = std::hash<std::string>()(node.symbol);
			size_t h2 = std::hash<size_t>()(static_cast<std::size_t>(node.side));
			return h1 ^ h2;
		}
	};


public:
	/**
	 * API for executing command. It parse the command and handle transaction to database.
	 *
	 * @param command A string in format of [timestamp; symbol; id; instruction; side; volume; price]
	 *
	 * @return true if command is in correct format and proper values. otherwise return false. 
	 */
	inline bool execute_command(const std::string& command) noexcept;

	/**
	 * API for getting number of orders per symbol.
	 *
	 * @return A map in format of  [Symbol => count of order]
	 */
	inline std::unordered_map<std::string, size_t> get_orders_count() const noexcept;

	/**
	 * API for getting K biggest buy order of a symbol respect to volume.
	 *
	 * @param Symbol that want to fetch its biggest order
	 * @param K indicates number of biggest order to be fetched.
	 *
	 * @return A vector of volume of biggest buy order.
	 */
	inline std::vector<size_t> get_biggest_buy_order(const std::string& symbol, size_t k = 3) const noexcept;

	/**
	 * API for getting best sell price based for specific time.
	 *
	 * @param Symbol that want to fetch its biggest order
	 * @param time that we want to fetch its best sell price
	 *
	 * @return A tuple in format of [price, volume, {is symbol exist or have sell transaction at that time or not}]
	 */
	inline std::tuple<size_t, size_t, bool> get_best_sell_at_time(const std::string& symbol, const std::string& time) const noexcept;

private:
	typedef std::unordered_map<Key, PriorityQueue<Order, uint32_t>, hash_fn> Table;

	inline static Key fill_key(ConstStringArray<COMMAND_PART_COUNT>& parts) noexcept;

	inline static Order fill_order(ConstStringArray<COMMAND_PART_COUNT>& parts) noexcept;

	inline static Instruction get_instruction(ConstStringArray<COMMAND_PART_COUNT>& parts) noexcept;

	template<int N>
	inline static StringArray<N> split(const std::string& info, const std::string& delimiter = ";") noexcept;
	
	Table table;
};

#include "DBManager-inl.h"

#endif
