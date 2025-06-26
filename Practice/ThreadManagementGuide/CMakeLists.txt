# ThreadManagementGuide/CMakeLists.txt

# Create executable
add_executable(ThreadManagementGuide
    main.cpp
    # add more .cpp files here as needed
)

# Optionally include directories if needed
# target_include_directories(ThreadManagementGuide PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Optionally link libraries
# target_link_libraries(ThreadManagementGuide PRIVATE Threads::Threads)

# Enable warnings (optional)
target_compile_options(ThreadManagementGuide PRIVATE -Wall -pedantic)
