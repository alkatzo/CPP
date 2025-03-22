cmake_minimum_required(VERSION 3.10)
project(client_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost (required by both server and client)
find_package(Boost REQUIRED COMPONENTS system)

# Add subdirectories for server and client
add_subdirectory(server)
add_subdirectory(client)

