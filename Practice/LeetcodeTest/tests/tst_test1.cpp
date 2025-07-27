#include <gtest/gtest.h>

#include <iostream>
#include <unordered_map>
#include <array>
#include <stack>
#include <string>
#include <list>
#include <algorithm>
#include <numeric>

using namespace testing;

TEST(TwoSum, Test1)
{
    constexpr std::array<int, 10> nums{2,4,1,6,8,9,5,2,1,3};
    constexpr int target = 7;

    std::unordered_map<int, size_t> seen;

    using resT = std::pair<size_t, size_t>;
    resT pair{};

    for(size_t i = 0; i < nums.size(); i++) {
        seen[nums[i]] = i;
        int look_for = target - nums[i];
        if(seen.contains(look_for)) {
            pair.first = i;
            pair.second = seen[look_for];
            break;
        }
    }

    resT res(3, 2);


    EXPECT_EQ(pair, res);
}

bool checkValidPar(const std::string& str) {
    auto matches = [](char l, char r) {
        return l == '{' && r == '}'
               || l == '[' && r == ']'
               || l == '(' && r == ')';
    };

    auto is_opening = [](char c) {
        return c == '(' || c == '{' || c == '[';
    };

    auto is_closing = [](char c) {
        return c == ')' || c == '}' || c == ']';
    };

    std::stack<char> stack;

    for(char c : str) {
        if(is_opening(c)) {
            stack.push(c);
        }
        else if(is_closing(c)) {
            if(stack.empty() || !matches(stack.top(), c)) {
                return false;
            }
            stack.pop();
        }
        else return false; // invalid char
    }

    return stack.empty();
}

TEST(ValidPar, Test1)
{
    EXPECT_EQ(checkValidPar(std::string{"{({()})}"}), true);
    EXPECT_EQ(checkValidPar(std::string{"{(){())}}"}), false);
    EXPECT_EQ(checkValidPar(std::string{"({)}"}), false);
}


std::list<int> mergeLists(const std::list<int>& l, const std::list<int>& r) {
    std::list<int> res;

    auto lit = l.begin();
    auto rit = r.begin();
    while(lit != l.end() && rit != r.end()) {
        if(*lit < *rit) {
            res.push_back(*lit);
            ++lit;
        }
        else {
            res.push_back(*rit);
            ++rit;
        }
    }

    std::copy(lit, l.end(), std::back_inserter(res));
    std::copy(rit, r.end(), std::back_inserter(res));

    return res;
}

std::vector<double> filter_signals(const std::vector<double>& signals, double threshold, double hi) {
    std::vector<double> res;

    std::copy_if(signals.begin(), signals.end(), std::back_inserter(res), [&threshold](const double& s){
        return s > threshold;
    });

    std::vector<double> res_clamped;
    std::transform(res.begin(), res.end(), std::back_inserter(res_clamped), [&threshold, &hi](const double& s){
        return std::clamp(s, threshold, hi);
    });

    return res_clamped;
}


struct Trade {
    std::string ticker;
    int volume;
    double price;
};

std::unordered_map<std::string, int> aggregate_volume(const std::vector<Trade>& trades) {
    std::unordered_map<std::string, int> volume_map;

    for(const auto& trade : trades) {
        volume_map[trade.ticker] += trade.volume;
    }

    return volume_map;
}

std::vector<double> moving_average(const std::vector<double>& prices, size_t window) {
    if(prices.empty() || window == 0 || window > prices.size())
        return {};

    std::vector<double> moving_avs;
    moving_avs.resize(prices.size() - window + 1);
    double mov_av = std::accumulate(prices.begin(), prices.begin() + window, 0.0) / window;
    moving_avs[0] = mov_av;

    for(size_t i = 1; i < prices.size() - window + 1; i++) {
        mov_av = (mov_av - prices[i-1] + prices[i+window-1])/window;
        moving_avs[i] = mov_av;
    }

    return moving_avs;
}


















