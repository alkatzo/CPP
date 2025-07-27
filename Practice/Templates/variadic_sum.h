#pragma once
#include <concepts>


template<std::integral ... Args>
constexpr auto variadic_sum(Args ... args) {
	return (args + ...);
}


