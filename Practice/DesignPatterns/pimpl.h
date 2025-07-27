#pragma once
#include <memory>

namespace pimpl {

template<typename T>
class PImpl
{
public:
    PImpl();
    ~PImpl();

    template<typename ... Args>
    PImpl(Args&& ...args);

    T* operator->();
    T& operator*();


private:
    std::unique_ptr<T> impl;
};


}


namespace client_pimpl
{

class Foo
{
    class impl;
    pimpl::PImpl<impl> impl;

public:
    void greetFoo();
};


}
