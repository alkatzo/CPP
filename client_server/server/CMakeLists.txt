# Server-specific CMake file
project(Server)

# Add Boost include directories
include_directories(${Boost_INCLUDE_DIRS})

# Add the server executable
add_executable(server server.cpp)

# Link Boost and pthread for the server executable
target_link_libraries(server ${Boost_LIBRARIES} pthread)

