# CMakeLists.txt

# Create executable
add_executable(Test
    main.cpp
    # add more .cpp files here as needed
)

# Enable warnings (optional)
target_compile_options(Test PRIVATE -Wall -pedantic)
