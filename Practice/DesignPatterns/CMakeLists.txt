# CMakeLists.txt

# Create executable
add_executable(DesignPatterns
    main.cpp
    visitor.cpp
    # add more .cpp files here as needed
)

# Enable warnings (optional)
target_compile_options(DesignPatterns PRIVATE -Wall -pedantic)
