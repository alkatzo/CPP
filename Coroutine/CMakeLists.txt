cmake_minimum_required(VERSION 3.20)
project(CoroutineEventLoop CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable verbose build output (optional)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(coroutine_event_loop main.cpp)

# Link pthread for std::thread
find_package(Threads REQUIRED)
target_link_libraries(coroutine_event_loop PRIVATE Threads::Threads)
