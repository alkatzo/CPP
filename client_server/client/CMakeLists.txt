# Client-specific CMake file
project(Client)

# Add Boost include directories
include_directories(${Boost_INCLUDE_DIRS})

# Add the client executable
add_executable(client client.cpp)

# Link Boost and pthread for the client executable
target_link_libraries(client ${Boost_LIBRARIES} pthread)

