cmake_minimum_required(VERSION 3.10)
project(MarketDataSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

# Create executable
add_executable(market_data_simulator 
    main.cpp
    MarketData.cpp
    FeedStatistics.cpp
)

# Link against threading library
target_link_libraries(market_data_simulator Threads::Threads)

# Set compiler flags for better performance
if(MSVC)
    target_compile_options(market_data_simulator PRIVATE /O2)
else()
    target_compile_options(market_data_simulator PRIVATE -O2)
endif() 
