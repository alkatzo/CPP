#include <cstdint>
#include <string>
#include "types.h"


struct Order {
    std::string id;
    OrderTraits::PriceT price;
    OrderTraits::QtyT qty;
    uint64_t timestamp;
};




