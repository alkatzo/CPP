#include <iostream>

#include "VariadicBuilder.h"
#include "wrapper.h"
#include "person.h"


template<typename T>
void print_type(T&& arg) {
    std::cout << typeid(T).name() << "\n";
    std::cout << typeid(std::decay_t<T>).name() << "\n";
}


int main()
{
    print_type("hello");


    VariadicBuilder builder;

    BigClass bo = builder.build<BigClass>(1, 2, std::string("Sasha"));

    Wrapper w1(1, 3.14);
    Wrapper w2("hello", 3.14);

    bridge::Person person{"Sasha"};
    person.greet();
}

