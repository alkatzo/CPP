// Templates.cpp : Source file for your target.
//

#include "Templates.h"

#include <algorithm>
#include <sstream>

template<typename T, typename P>
auto max(const T& a, const P& b) {
    return  (a > b) ? a : b;
}

template<typename T, typename P>
auto min(const T& a, const P& b) -> decltype ((a < b) ? a : b);

int main()
{
    // Foo f1{1};
    // Foo f2{2};

    // std::swap(f1, f2);

    // std::swap(&f1, &f2);

    // std::ostringstream os;
    // os << 1;

    auto r = max(1l, 2);
    r = min(1l, 2);

    decltype(max(1l, 2)) h = 3l;

    return 0;
}

template<typename T, typename P>
auto min(const T& a, const P& b) -> decltype ((a < b) ? a : b) {
    return  (a < b) ? a : b;
}
