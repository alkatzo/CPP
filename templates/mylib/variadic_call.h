#ifndef VARIADIC_CALL_H
#define VARIADIC_CALL_H

#include <iostream>

class Context {

};

template<typename RetType, typename C, typename... Params>
RetType call(C* o, RetType (C::*method)(Context, Params...), const Params&... args) {
    Context c;
    return (o->*method)(c, args...);
}

template<typename Type, typename Del>
std::ostream& join(std::ostream& os, Del del, Type arg) {
    os << arg;
    return os;
}

template<typename Type, typename Del, typename... Types>
std::ostream& join(std::ostream& os, Del del, Type arg, Types... args) {
    os << arg << del;
    join(os, del, args...);
    return os;
}

#endif // VARIADIC_CALL_H
