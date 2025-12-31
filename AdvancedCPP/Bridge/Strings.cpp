#include <string>
#include <iostream>


int main(int argc, char** argv) {
    std::string triple_x(3, 'x');

    std::string hello("Hello");
    std::string hello1(hello, 1);
    std::string hello2(hello, 1, 2);


    std::cout << triple_x << std::endl;
    std::cout << hello1 << std::endl;
    std::cout << hello2 << std::endl;


    size_t o = hello.find('o');
    size_t or1 = hello.find("or");
    size_t O = hello.find('O');
    if(O == std::string::npos)
        std::cout << ("'O' not found") << std::endl;

    if(o != std::string::npos) {
        hello[0] = 'p';
    }

    size_t re = hello.rfind('e');
    if(re != std::string::npos)
        std::cout << ("'e' found") << std::endl;

    std::cout << hello << std::endl;

    std::string vowels{"aeiou"};

    size_t v = hello.find_first_of(vowels);
    size_t nv = hello.find_first_not_of(vowels);

    std::cout << "vowel at " << v << " consonant at " << nv << std::endl;


    hello.append(" World");

    hello.insert(5, "lde");
    std::cout << hello << std::endl;

    std::string word{"word"};
    auto last = std::end(word) - 1;

    word.insert(last, 'l');

    auto pastlast = std::end(word);
    word.insert(pastlast, 's');

    std::cout << word << std::endl;

    hello = "Hello";
    hello.erase(1, 2);
    std::cout << hello << std::endl;
}
