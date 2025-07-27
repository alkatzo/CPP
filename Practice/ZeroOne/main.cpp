#include <iostream>
#include <vector>
#include <list>
#include <ranges>
#include <map>
#include <functional>
#include <cassert>
#include <concepts>
#include <future>

#include "buffer.h"

std::vector<int> evenSquared(const std::vector<int>& numbers) {
    auto view = numbers
                | std::views::filter([](int i) { return i % 2 == 0; })
                | std::views::transform([](int i) { return i * i; });
    return {view.begin(), view.end()};
}

void spliceLists(std::list<int>& l1, auto it, std::list<int>& l2, auto l2begin, auto l2end) {
    l1.splice(it, l2, l2begin, l2end);
}

template<typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
};

template<typename Compare, typename T>
concept Comparator = requires(Compare comp, T a, T b) {
    { comp(a, b) } -> std::convertible_to<bool>;
};

template<Comparable T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    assert(lo <= hi && "Invalid clamp range lo > hi");
    return std::min(std::max(v, lo), hi);
}

template<typename T, typename Compare>
requires Comparator<Compare, T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp) {
    assert(lo <= hi && "Invalid clamp range lo > hi");
    return comp(v, lo) ? lo : (comp(v, hi) ? v : hi);
}

template<typename... T>
constexpr auto foldSum (T... args) {
    return (... + args); // ((s1 + s2) + s3) ...
}

long long factorial(size_t n) {
    long long f = 1;
    for(size_t i = 1; i < n; i++) {
        f *= i;
    }

    return f;
}

template<typename T>
concept IntegralArg = std::integral<std::remove_cvref_t<T>>;

template<IntegralArg T>
auto foo(T&& i) -> std::remove_cvref_t<T> {
    return std::forward<T>(i);
}


int main()
{
    auto res = evenSquared({1,2,3,4,5});

    std::list<int> l1{1,2,3};
    std::list<int> l2{4,5,6,7};

    auto l2begin = l2.begin();
    auto l2end = l2.begin();
    std::advance(l2end, 2);
    spliceLists(l1, std::next(l1.begin()), l2, l2begin, l2end);

    Buffer buf{"Preved"};

    std::vector<Buffer> bufs;
    bufs.resize(4);
    bufs[0] = std::move(buf);

    auto comparator = [](const std::string& l, const std::string& r) {
        return l.size() == r.size() ? std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end()) : l.size() < r.size();
    };


    struct LexSizeComparator {
        bool operator()(const std::string& l, const std::string& r) const {
            return l.size() == r.size()
            ? std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end())
            : l.size() < r.size();
        }
    };

    std::map<std::string, int, std::function<bool(const std::string& l, const std::string& r)>> map1(comparator);
    std::map<std::string, int, LexSizeComparator> map2;

    map1["cat"] = 1;
    map1["dog"] = 2;
    map1["zebra"] = 3;

    for(auto& pair : map1) {
        std::cout << pair.first << " : " << pair.second << " ";
    }

    std::cout << std::endl;

    map2["dog"] = 1;
    map2["cat"] = 2;
    map2["zebra"] = 3;

    for(auto& pair : map2) {
        std::cout << pair.first << " : " << pair.second << " ";
    }

    std::cout << std::endl;


    constexpr int result1 = clamp(2+4, 3, 9);

    auto comp = [](int a, int b) {
        return a < b;  // Custom logic: shorter string is "less"
    };

    constexpr int result2 = clamp(2+4, 3, 9, comp);


    constexpr int sum = foldSum(1,2,3);


    auto future = std::async(std::launch::async, &factorial, 10);

    long long fres = future.get();

    std::cout << fres << std::endl;


    foo(3);

    int i = 3;
    foo(i);

    return 0;
}
