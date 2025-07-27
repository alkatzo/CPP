#pragma once
#include <utility>

template<typename T, typename V>
class Wrapper {
private:
    T first;
    V second;


public:
    Wrapper(T&& t, V&& v)
        : first{std::forward<T>(t)}, second{std::forward<V>(v)} {

    }
};

/*
 When someone constructs a Wrapper with arguments of types T and V, deduce the class template parameters as std::decay_t<T> and std::decay_t<V>
Here's what std::decay_t<T> actually does:
For a given type T, it:

Removes reference (&, &&)

Removes cv-qualifiers (const, volatile)

If T is an array, converts it to a pointer to the element type

If T is a function type, converts it to a function pointer
 */
template<typename T, typename V>
Wrapper(T&&, V&&) -> Wrapper<std::decay_t<T>, std::decay_t<V>>;
