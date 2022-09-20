#include "caller.h"

#include <iostream>

Caller::Caller()
{
}

int Caller::callFoo(const int& i, const char& c)
{
    int res = call(this, &Caller::foo, i, c);
    return res;
}

int Caller::foo(Context ctx, int i, char c)
{
    return i;
}
