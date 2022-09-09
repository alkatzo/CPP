#include "caller.h"

#include <iostream>

Caller::Caller()
{
}

int Caller::callFoo(int, char)
{
    int res = call(this, &Caller::foo, 42, 'c');
    return res;
}

int Caller::foo(Context ctx, int i, char c)
{
    return i;
}
