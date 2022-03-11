#include <gtest/gtest.h>
#include <iostream>

#include "DBManagerTest.h"
#include "PriorityQueueTest.h"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
