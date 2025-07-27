#pragma once
#include <string>

namespace bridge {

class Person
{
public:
    Person(const std::string& name);
    ~Person();
    void greet();

private:
    std::string name;

    class PersonImpl;
    PersonImpl* pImpl;
};

}
