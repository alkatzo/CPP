# CMakeLists.txt

# Create executable
add_executable(WorkerActivity
    main.cpp
    # add more .cpp files here as needed
)

# Enable warnings (optional)
target_compile_options(WorkerActivity PRIVATE -Wall -pedantic)
