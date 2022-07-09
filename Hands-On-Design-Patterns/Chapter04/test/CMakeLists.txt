
enable_testing()

add_executable(test test.cpp)

# Now simply link against gtest or gtest_main as needed. Eg
target_link_libraries(test gtest_main)

include(GoogleTest)
gtest_discover_tests(test)

