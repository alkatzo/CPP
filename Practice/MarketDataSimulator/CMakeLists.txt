cmake_minimum_required(VERSION 3.10)
project(MarketDataSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

# Include FetchContent for Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Create main executable
add_executable(market_data_simulator 
    main.cpp
    MarketData.cpp
    FeedStatistics.cpp
    MarketData.h
    MarketDataFeed.h
    MarketDataReader.h
    FeedStatistics.h
    LockingStrategy.h
    LockGuard.h
    SynchronizedQueue.h
)

# Create test executable
add_executable(locking_strategy_tests
    tests/test_main.cpp
    tests/LockingStrategyTests.cpp
    # tests/PerformanceTests.cpp
    tests/LockingStrategyTests.h
    # tests/PerformanceTests.h
    tests/TestFramework.h
    MarketData.cpp
    FeedStatistics.cpp
    MarketData.h
    MarketDataFeed.h
    MarketDataReader.h
    FeedStatistics.h
    LockingStrategy.h
    LockGuard.h
    SynchronizedQueue.h
)

# Link against threading library and Google Test
target_link_libraries(market_data_simulator Threads::Threads)
target_link_libraries(locking_strategy_tests Threads::Threads gtest gtest_main)

# Set compiler flags for better performance
if(MSVC)
    target_compile_options(market_data_simulator PRIVATE /O2)
    target_compile_options(locking_strategy_tests PRIVATE /O2)
else()
    target_compile_options(market_data_simulator PRIVATE -O2)
    target_compile_options(locking_strategy_tests PRIVATE -O2)
endif()

# Add test target
add_custom_target(test
    COMMAND locking_strategy_tests
    DEPENDS locking_strategy_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
) 
