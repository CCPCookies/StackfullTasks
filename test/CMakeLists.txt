cmake_minimum_required(VERSION 3.31)

enable_testing()

find_package(GTest CONFIG REQUIRED)

set(SRC_FILES
    src/SimpleTaskTest.cpp
    src/TaskTest.cpp
    src/TestFixture.cpp
    src/TestFixture.h
)

add_executable(stackfullTasksTest ${SRC_FILES})

target_link_libraries(stackfullTasksTest stackfullTasks GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(stackfullTasksTest TaskletppTest)

target_compile_features(stackfullTasksTest PRIVATE cxx_std_17)