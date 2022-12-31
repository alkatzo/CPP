
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <string>
#include <iostream>
#include <utility>

using namespace testing;

// 1
void g(std::string& i) {
    std::cout << "void g(int& i)" << std::endl;
}

// 2
void g(const std::string& i) {
    std::cout << "void g(const int& i)" << std::endl;
}

// 3
void g(std::string&& i) {
    std::cout << "void g(int&& i)" << std::endl;
}

template<typename T>
void f(T&& val) {
    g(val);
    g(std::forward<T>(val));
}


TEST(TestSuitForward, TestForward)
{
    // 3
    f(std::string("Preved"));

    // 1
    std::string s;
    f(s);

    // 2
    const std::string s1;
    f(s1);
}


