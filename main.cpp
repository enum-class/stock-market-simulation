#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "DBManager.h"

using namespace std;

void print(const string& prefix, const vector<size_t>& elements)
{
	cout << prefix << endl;

	for (auto& element : elements)
		cout << element << "\t";

	cout << endl;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		cout << "Give an input file\n";
		return 0;
	}

	ifstream infile(argv[1]);
	string order;
	
	DBManager manager;

	while (getline(infile, order))
		manager.execute_command(order);

	unordered_map<string, size_t> orders_count = manager.get_orders_count();
	cout << "Orders count : " << endl;
	for (auto& element : orders_count)
		cout << element.first << "\t" << element.second << endl;

	vector<size_t> biggest = manager.get_biggest_buy_order("DVAM1");
	print("Biggest buy order for symbol \"DVAM1\" : ", biggest);

	size_t price;
	size_t volume;
	bool is_valid;

	std::tie(price, volume, is_valid) = manager.get_best_sell_at_time("DVAM1", "15:30:00");

	if (is_valid)
		cout << "Best sell price at time 15:30:00 for symbol DVAM1 is {" << price << "} and its volume is {" << volume << endl;
	else
		cout << "Best sell price at time 15:30:00 for symbol DVAM1 Not found" << endl;
}
