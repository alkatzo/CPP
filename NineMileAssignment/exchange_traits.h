#pragma once
#include <unistd.h>

template<size_t SYMBOL_SIZE = 3, size_t EXCHANGE_SIZE = 4>
struct ExTraits {
    static constexpr size_t SymbolSize   = SYMBOL_SIZE;
    static constexpr size_t ExchangeSize = EXCHANGE_SIZE;
};
