#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>

namespace factory {

enum class COLOR {RED, BLUE, GREEN};
enum class SIZE {BIG, SMALL};

class OfflineProduct;
class OnlineProduct;

class Product {
protected:
    Product(std::string name, COLOR color, SIZE size) : name{std::move(name)}, color{color}, size{size} {}
public:
    std::string name{};
    COLOR color{};
    SIZE size{};

    static OfflineProduct newOfflineProduct(std::string name, COLOR color, SIZE size);
    static OnlineProduct newOnlineProduct(std::string name, COLOR color, SIZE size);

    friend std::ostream& operator<<(std::ostream& os, const Product& p) {
        os << "Product : " << p.name << " " << static_cast<int>(p.color) << " " << static_cast<int>(p.size);
        return os;
    }
};

class OfflineProduct : public Product {
public:
    OfflineProduct(std::string name, COLOR color, SIZE size) : Product(name, color, size) {}
};

class OnlineProduct : public Product {
public:
    OnlineProduct(std::string name, COLOR color, SIZE size) : Product(name, color, size) {}
};

OfflineProduct Product::newOfflineProduct(std::string name, COLOR color, SIZE size) {
    return OfflineProduct(name, color, size);
}

OnlineProduct Product::newOnlineProduct(std::string name, COLOR color, SIZE size) {
    return OnlineProduct(name, color, size);
}

class ProductFactory {
    std::unordered_map<std::string, std::function<Product(std::string, COLOR, SIZE)>> factoryMap;

public:
    ProductFactory() {
        factoryMap["online"] = [](std::string name, COLOR color, SIZE size){ return OnlineProduct(name, color, size);};
        factoryMap["offline"] = [](std::string name, COLOR color, SIZE size){ return OfflineProduct(name, color, size);};
    }
};

void test() {
    auto p = Product::newOfflineProduct("hello", COLOR::BLUE, SIZE::BIG);
    std::cout << p << std::endl;
}

}
