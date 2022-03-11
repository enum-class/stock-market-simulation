#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "../DBManager.h"

using namespace std;

TEST(DB, execute_insert)
{
	DBManager manager;
	manager.execute_command("09:00:00.440000;DVAM1;2837174;I;SELL;72;36.30");
	std::unordered_map<string, size_t> orders_count = manager.get_orders_count();

	ASSERT_EQ(orders_count.size(), 1);
}

TEST(DB, execute_cancel)
{
	DBManager manager;
	manager.execute_command("09:00:00.440000;DVAM1;2837174;I;BUY;72;36.30");
	manager.execute_command("09:00:00.440000;DVAM1;2837174;C;BUY;72;36.30");
	std::unordered_map<string, size_t> orders_count = manager.get_orders_count();
	ASSERT_EQ(orders_count["DVAM1"], 0);
}

TEST(DB, execute_update)
{
	DBManager manager;
	manager.execute_command("09:00:00.440000;DVAM1;2837174;I;BUY;72;36.30");
	manager.execute_command("09:00:00.440000;DVAM1;2837174;A;BUY;12;10.30");
	manager.execute_command("09:00:00.440000;DVAM1;2837170;I;SELL;12;10.30");

	std::unordered_map<string, size_t> orders_count = manager.get_orders_count();

	ASSERT_EQ(orders_count["DVAM1"], 2);
}
