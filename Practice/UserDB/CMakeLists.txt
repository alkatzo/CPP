# CMakeLists.txt

# Create executable
add_executable(UserDB
    main.cpp
    # add more .cpp files here as needed
)

# Enable warnings (optional)
target_compile_options(UserDB PRIVATE -Wall -Wextra -pedantic)
