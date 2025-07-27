add_executable(my_tests test_main.cpp
    tst_test1.cpp)
target_link_libraries(my_tests gtest_main)
include(GoogleTest)
gtest_discover_tests(my_tests)
