#ifndef CALLER_H
#define CALLER_H

#include "variadic_call.h"

class Caller
{
public:
    Caller();

    int callFoo(const int &i, const char &c);
    int foo(Context ctx, int i, char c);
};

#endif // CALLER_H
