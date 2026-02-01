cmake_minimum_required(VERSION 3.20)
project(ProducerConcumer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find required packages
find_package(Threads REQUIRED)

# Include FetchContent for Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

# Create main executable
add_executable(producer_consumer
    main.cpp
)

# Create test executable
add_executable(producer_consumer_tests
    tests/test_main.cpp
)

# Link against threading library and Google Test
target_link_libraries(producer_consumer Threads::Threads)
target_link_libraries(producer_consumer_tests Threads::Threads gtest gtest_main)

# Set compiler flags for better performance
if(MSVC)
    target_compile_options(producer_consumer PRIVATE /O2)
    target_compile_options(producer_consumer_tests PRIVATE /O2)
else()
    target_compile_options(producer_consumer PRIVATE -O2)
    target_compile_options(producer_consumer_tests PRIVATE -O2)
endif()

# Add test target
add_custom_target(test
    COMMAND producer_consumer_tests
    DEPENDS producer_consumer_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
) 
