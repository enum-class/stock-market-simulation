cmake_minimum_required(VERSION 2.6)
project(basic_test)

################################
# GTest
################################
#ADD_SUBDIRECTORY (gtest-1.8.0)
enable_testing()
find_package(GTest REQUIRED)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file
add_executable( runUnitTests main.cpp )
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main pthread)

add_test( runUnitTests runUnitTests )

#target_link_libraries(test1 GTest::GTest GTest::Main)
#add_test(test-1 test1)
