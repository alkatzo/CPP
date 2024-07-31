#ifndef FUNC_RETURN_TYPE_H
#define FUNC_RETURN_TYPE_H

#include <string>
#include <type_traits>

namespace Chapter1 {

template<typename T>
std::string toString(T x) {
    return std::to_string(x);
}

template<typename R, typename T1, typename T2>
R max(T1 a, T2 b) {
    return a > b ? toString(a) : toString(b);
}

template<typename T1, typename T2>
auto max_auto_trailing_hint(T1 a, T2 b) -> typename std::decay_t<decltype(true ? a : b)> // deduce the return type from operator?() and decay to remove reference if a or b are references ( return by value )
{
    return a > b ? a : b;
}

template<typename T1, typename T2>
auto max_auto_trailing_hint_no_decay(T1 a, T2 b) -> decltype(true ? a : b) // deduce the return type from operator?() and decay to remove reference if a or b are references ( return by value )
{
    return a > b ? a : b;
}

template<typename T1, typename T2>
auto max_auto(T1 a, T2 b) // deduce the return type from return statement ( OK since c++14 )
{
    return a > b ? toString(a) : toString(b);
}

}

#endif // FUNC_RETURN_TYPE_H
