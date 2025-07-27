#include "person.h"
#include <iostream>

namespace bridge {

class Person::PersonImpl {
public:
    void greet(Person* p) {
        std::cout << "Hello" << p->name << std::endl;
    }
};

Person::Person(const std::string& name) : name{name}, pImpl{new PersonImpl} {}

Person::~Person()
{
    delete pImpl;
}

void Person::greet()
{
    pImpl->greet(this);
}

}
