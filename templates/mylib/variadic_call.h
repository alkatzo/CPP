#ifndef VARIADIC_CALL_H
#define VARIADIC_CALL_H

class Context {

};

template<typename RetType, typename C, typename... Params>
RetType call(C* o, RetType (C::*method)(Context, Params...), Params... args) {
    Context c;
    return (o->*method)(c, args...);
}

#endif // VARIADIC_CALL_H
