
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "caller.h"

using namespace testing;

TEST(TestSuit, TestCase)
{
    Caller c;
    int res = c.callFoo(42, 'a');
    EXPECT_EQ(res, 42);
    ASSERT_THAT(0, Eq(0));
}

TEST(TestSuit, TestJoin)
{
    std::ostringstream oss;
    join(oss, ", ", "Preved", 1, "Medved") << "\n";

    std::cout << oss.str();
}


