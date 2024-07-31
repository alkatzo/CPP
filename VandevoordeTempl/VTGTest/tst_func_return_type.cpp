
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "Src/Chapter1/func_return_type.h"

using namespace testing;

TEST(Chapter1, func_return_type_max)
{
    auto res = Chapter1::max<std::string>(1, 3.7);
    EXPECT_EQ(res, "3.700000");
    ASSERT_THAT(0, Eq(0));
}

TEST(Chapter1, func_return_type_max_auto_trailiing_hint)
{
    auto res = Chapter1::max_auto_trailing_hint(1, 3.7);
    EXPECT_EQ(res, 3.7);
}

TEST(Chapter1, func_return_type_max_auto_trailiing_hint_no_decay)
{
    int a = 1;
    double b = 3.7;
    int &ar = a;
    double &br = b;
    // auto res = Chapter1::max_auto_trailing_hint_no_decay(ar, br);
    static_assert(!std::is_reference_v<decltype(Chapter1::max_auto_trailing_hint_no_decay(ar, br))>, ""); // hmm.. even though the return type does not decay, the result is not ref... ask copilot
    // EXPECT_EQ(res, 3.7);
}

TEST(Chapter1, func_return_type_max_auto)
{
    auto res = Chapter1::max_auto(1, 3.7);
    EXPECT_EQ(res, "3.700000");
}
