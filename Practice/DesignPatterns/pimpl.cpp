#include "pimpl.h"
#include <iostream>

namespace pimpl {

template<typename T>
PImpl<T>::PImpl() : impl{new T{}}
{
}

template<typename T>
PImpl<T>::~PImpl()
{
}

template<typename T>
T *PImpl<T>::operator->()
{
    return impl.get();
}

template<typename T>
T &PImpl<T>::operator*()
{
    return *impl.get();
}

template<typename T>
template<typename ...Args>
PImpl<T>::PImpl(Args && ...args) : impl(T{std::forward<Args>(args)...})
{
}


}


namespace client_pimpl
{

class Foo::impl
{
public:

    void foo(Foo* f) {
        std::cout << "Foo::foo" << std::endl;
    }
};

void Foo::greetFoo()
{
    impl->foo(this);
}

}
