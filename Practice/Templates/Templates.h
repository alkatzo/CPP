// Templates.h : Header file for your target.

#pragma once

#include <utility>

class Foo {
public:
    Foo() {}

    Foo(int i) : i{i} {}

    Foo(const Foo& f) : i{f.i} {
    }

    Foo(Foo&& f) : i{f.i} {
        f.i = -1;
    }

    Foo& operator=(const Foo& f) {
        if(this != &f)
            i = f.i;
        return *this;
    }

    Foo& operator=(Foo&& f) {
        if(this != &f) {
            i = f.i;
            f.i = -1;
        }
        return *this;
    }


private:
    int i{-1};
};

namespace std {

// template<typename T>
// inline void swap(T* __a, T* __b)
// {
//     T __tmp = std::move(*__a);
//     *__a = std::move(*__b);
//     *__b = std::move(__tmp);
// }

// template<>
// inline void swap(Foo* __a, Foo* __b)
// {
//     Foo __tmp = std::move(*__a);
//     *__a = std::move(*__b);
//     *__b = std::move(__tmp);
// }

// template<>
// inline void swap(Foo& __a, Foo& __b)
// {
//     Foo __tmp = std::move(__a);
//     __a = std::move(__b);
//     __b = std::move(__tmp);
// }

}
