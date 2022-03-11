#ifndef DB_MANAGER_INL_H_
#define DB_MANAGER_INL_H_

#ifndef PRIORITY_QUEUE_H_
#error "DBManager-inl.h" should be included only in "DBManager.h" file
#endif

#include "Logger.h"

using namespace std;

DBManager::Key DBManager::fill_key(DBManager::ConstStringArray<COMMAND_PART_COUNT>& parts) noexcept
{
	enum FieldIndex
	{
		SYMBOL_INDEX = 1,
		SIDE_INDEX = 4,
	};
	
	DBManager::Key key;

	key.symbol = parts[SYMBOL_INDEX];
	if (parts[SIDE_INDEX] == "BUY")
		key.side = Side::BUY;
	else if (parts[SIDE_INDEX] == "SELL")
		key.side = Side::SELL;
	else
	{
		key.side = Side::UNKNOW;
		Logger::warning("DBManager::fill_key(): Unknown side.");
	}
	
	return key;
}

DBManager::Order DBManager::fill_order(DBManager::ConstStringArray<COMMAND_PART_COUNT>& parts) noexcept
{
	enum FieldIndex
	{
		TIME_INDEX = 0,
		ID_INDEX = 2,
		VOLUME_INDEX = 5,
		PRICE_INDEX = 6
	};

	DBManager::Order order;

	order.time = parts[TIME_INDEX];
	order.id = stol(parts[ID_INDEX]);
	order.volume = stol(parts[VOLUME_INDEX]);
	order.price = stod(parts[PRICE_INDEX]);

	if (order.price < 0)
		Logger::error("DBManager::fill_order(): Negative value for price.");

	return order;
}

DBManager::Instruction DBManager::get_instruction(DBManager::ConstStringArray<COMMAND_PART_COUNT>& parts) noexcept
{
	static constexpr size_t INSTRUCTION_INDEX = 3;

	const string instruction = parts[INSTRUCTION_INDEX];
	if (instruction == "I")
		return Instruction::INSERT;
	else if (instruction == "C")
		return Instruction::CANCEL;
	else if (instruction == "A")
		return Instruction::AMEND;
	else
		return Instruction::UNKNOW;
}

template<int N>
DBManager::StringArray<N> DBManager::split(const string& info, const string& delimiter) noexcept
{
	size_t start_index = 0;
	size_t end_index = info.find(delimiter, start_index);
	size_t index = 0;

	DBManager::StringArray<N> output;
	
	while ((end_index = info.find(delimiter, start_index)) != string::npos)
	{
		output[index++] = info.substr(start_index, end_index - start_index);
		start_index = end_index + delimiter.size();
	}

	output[index] = info.substr(start_index, end_index - start_index);
	return output;
}

bool DBManager::execute_command(const string& command) noexcept
{
	DBManager::StringArray<COMMAND_PART_COUNT> parts = split<COMMAND_PART_COUNT>(command);
	
	DBManager::Key key = fill_key(parts);
	DBManager::Order order = fill_order(parts);
	Instruction instruction = get_instruction(parts);
	
	if (instruction == Instruction::INSERT)
		table[key].insert(order);
	else if (instruction == Instruction::CANCEL)
	{
		table[key].remove(order);
	}
	else if (instruction == Instruction::AMEND)
	{
		table[key].update(order);
	}
	else
	{
		Logger::error("DBManager::execute_command(): Unknown Instruction.");
	}
}

unordered_map<string, size_t> DBManager::get_orders_count() const noexcept
{
	unordered_map<string, size_t> counts;

	for (auto& row : table)
		counts[row.first.symbol] += row.second.get_orders_count();

	return counts;
}

vector<size_t> DBManager::get_biggest_buy_order(const string& symbol, size_t k) const noexcept
{
	Key key = {symbol, Side::BUY};

	auto search = table.find(key);
	if (search == table.end())
	{
		Logger::warning("DBManager::get_biggest_buy_order(): symbol %s  not found.", symbol.c_str());
		return vector<size_t>();
	}

	vector<size_t> orders_volume;
	for (auto& order : search->second.get_top_items(k))
		orders_volume.push_back(order.volume);

	return orders_volume;
}

tuple<size_t, size_t, bool> DBManager::get_best_sell_at_time(const string& symbol, const string& time) const noexcept
{
	static auto match = [](const DBManager::Order& order, const string& time) {
		//return order.time.find(time) != string::npos;
		return order.time < time;
	};

	static auto compare = [](const DBManager::Order& order1, const DBManager::Order& order2)
	{
		return order1.price < order2.price;
	};

	Key key = {symbol, Side::SELL};

	auto search = table.find(key);
	if (search == table.end())
	{
		Logger::warning("DBManager::get_best_sell_at_time(): symbol %s  not found.", symbol.c_str());
		return make_tuple(0, 0, false);
	}

	auto order_pair = table.at(key).filter(time, match, compare);
	return make_tuple(order_pair.first.price, order_pair.first.volume , order_pair.second);
}

#endif
