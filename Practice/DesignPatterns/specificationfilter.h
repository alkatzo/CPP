#pragma once
#include <vector>
#include <string>
#include <algorithm>

enum COLOR {RED, BLUE, GREEN};
enum SIZE {BIG, SMALL};


class Product {
public:
    std::string name;
    COLOR color;
    SIZE size;
};


template<typename T>
class Specification
{
public:
    virtual bool is_satisfied(T* item) const = 0;
};

template<typename T>
class Filter
{
public:
    virtual std::vector<T*> filter(const std::vector<T*> items, Specification<T>& spec) = 0;
};

class BetterFilter : Filter<Product>
{
    // Filter interface
public:
    std::vector<Product *> filter(std::vector<Product *> items, Specification<Product> &spec) {
        std::remove_if(items.begin(), items.end(), [&spec](Product* p) { return !spec.is_satisfied(p);});
        return items;
    }
};

class ColorSpecification : public Specification<Product>
{
public:
    ColorSpecification(COLOR color) : color{color} {

    }

    virtual bool is_satisfied(Product* item) const override {
        return item->color == color;
    }

private:
    COLOR color;
};

template<typename T>
class AndSpecification : public Specification<T>
{
public:
    AndSpecification(Specification<T>& f, Specification<T>& s) : first{f}, second{s} {
    }

    virtual bool is_satisfied(T* item) const override {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }

private:
    Specification<T>& first;
    Specification<T>& second;
};


std::vector<Product*> filter(std::vector<Product*> items, Specification<Product>& spec1, Specification<Product>& spec2) {
    AndSpecification<Product> spec(spec1, spec2);
    return BetterFilter().filter(items, spec);
}




