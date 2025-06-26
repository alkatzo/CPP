# CMakeLists.txt

# Create executable
add_executable(ShoppingMall
    main.cpp
    # add more .cpp files here as needed
)

# Enable warnings (optional)
target_compile_options(ShoppingMall PRIVATE -Wall -pedantic)
