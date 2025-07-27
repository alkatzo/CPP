#pragma once
#include <concepts>

template<typename T>
concept HasSize = requires(T x) {
    { x.size() }  -> std::convertible_to<std::size_t>;
};
