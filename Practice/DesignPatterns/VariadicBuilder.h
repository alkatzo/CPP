#pragma once
#include <utility>
#include <string>

class VariadicBuilder
{
public:

	template<typename R, typename... Args>
	R build(Args&& ...args) {
		return R(std::forward<Args>(args)...);
	}
};


class BigClass {
private:
    int i;
    int j;
    std::string name;

public:
    BigClass(int i, int j, std::string name) : i{i}, j{j}, name{std::move(name)} {

    }

};
