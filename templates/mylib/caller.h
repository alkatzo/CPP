#ifndef CALLER_H
#define CALLER_H

#include "variadic_call.h"

class Caller
{
public:
    Caller();

    int callFoo(int, char);
    int foo(Context ctx, int i, char c);
};

#endif // CALLER_H
