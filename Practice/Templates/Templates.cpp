// Templates.cpp : Source file for your target.
//

#include "Templates.h"

#include <iostream>
#include <memory>
#include <set>
#include <map>
#include <unordered_map>
#include <functional>

template<typename T, typename P>
auto max(const T& a, const P& b) {
    return  (a > b) ? a : b;
}

template<typename T, typename P>
auto min(const T& a, const P& b) -> decltype ((a < b) ? a : b);

auto average(auto a, auto b) {
    return (a + b) / 2.0;
}


// adding decltype(auto) makes the function to return the exact type, including the references
// If container.begin() returns a reference (int&), decltype(auto) will deduce int&
// If you used just auto, it would decay the reference and return a copy (int).
template <typename T>
decltype(auto) get_first(T&& container) {
    return *container.begin();
}

// Preserve Value Category
// This forwards x exactly as it came in — preserving lvalues/rvalues.
template<typename T>
decltype(auto) forward_identity(T&& x) {
    return std::forward<T>(x);
}

// Wrapping Existing APIs
// This perfectly forwards c, and returns back() with the exact type it returns — whether it’s an int&, const int&, or int&&.
template<typename Container>
decltype(auto) get_back(Container&& c) {
    return std::forward<Container>(c).back();
}

void func_double(double) {}
void func_integral(int) {}

template<typename T>
std::unique_ptr<T> accept_unique_ptr(std::unique_ptr<T> ptr) {
    return ptr;
}

struct Base {
public:
    virtual void help() {}
};

struct Derived : public Base {
public:
    int i;
};

struct Node : public std::enable_shared_from_this<Node> {
    Node(const std::string& name) : name{name} {}
    ~Node() {
        std::cout << name << " destroyed" << std::endl;
    }

    virtual std::string getName() {
        return name;
    }

    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // prevent cycle
};

struct DerivedNode : public Node {
    DerivedNode(const std::string& name) : Node(name) {}

    virtual std::string getName() override {
        return name + " Derived";
    }
};


int main()
{
    // Foo f1{1};
    // Foo f2{2};

    // std::swap(f1, f2);

    // std::swap(&f1, &f2);

    // std::ostringstream os;
    // os << 1;

    auto r = max(1l, 2);
    r = min(1l, 2);

    decltype(max(1l, 2)) h = 3l;

    auto a = average(1l, 4);

    /*
    auto get1 = *container.begin();        // returns by value
    decltype(auto) get2 = *container.begin(); // returns by reference if `begin()` does
    */

    auto func = []<typename T>(T a, T b) {
        return a + b;
    };


    // Type traits
    auto i{45};

    std::cout << std::boolalpha;
    std::cout << std::is_integral_v<decltype(i)> << std::endl;

    if constexpr(std::is_integral_v<decltype(i)>) {
        func_integral(i);
    }
    else if constexpr(std::is_floating_point_v<decltype(i)>) {
        func_double(i);
    }

    // Unique ptrs
    auto upint = std::make_unique<int>(1);
    auto upint1 = accept_unique_ptr(std::move(upint));

    // Shared ptrs
    std::shared_ptr<int> sp1 = std::move(upint1);

    // Week ptr use case
    {
        auto a = std::make_shared<Node>("a");
        auto b = std::make_shared<DerivedNode>("b");

        std::cout << "init. a: " << a.use_count() << " b: " << b.use_count() << std::endl;

        a->next = b;
        b->prev = a;

        // auto c = a->shared_from_this(); // a ref count becomes 2

        // a.reset();

        if (auto sp = b->prev.lock()) {
            std::cout << "in lock. a: " << a.use_count() << " b: " << b.use_count() << std::endl;
        } else {
            std::cout << "Previous has been destroyed.\n";
        }

        std::cout << "done. a: " << a.use_count() << " b: " << b.use_count() << std::endl;
    }

    std::cout << "size of Base : " << sizeof(Base) << std::endl;  // 1 without vurtual table, 8 with virtual table
    std::cout << "size of Derived : " << sizeof(Derived) << std::endl; // 8 without vurtual table, 16 with virtual table

    Derived d;
    Base b = d;

    std::set<int, std::function<bool(int, int)>> numbers([](int l, int r) { return l < r; });
    numbers.insert({11, 2, 3, 66, 345});

    auto it = numbers.find(2);

    if(it != numbers.end()) {
        std::cout << "Found" << std::endl;
    }

    it = std::find_if(numbers.begin(), numbers.end(), [](int key) {return key == 2;});
    if(it != numbers.end()) {
        std::cout << "Found" << std::endl;
    }

    std::vector<int> v1{1,2,3,4,5};
    std::vector<int> v2;

    std::sort(v1.begin(), v1.end(), [](int l, int r) { return l > r;} );

    std::copy(v1.begin(), v1.begin() + 3, std::back_inserter(v2));
    std::cout << "Size of v2  : " << v2.size() <<  std::endl;

    std::transform(v1.begin(), v1.end(), std::back_inserter(v2), [](int e) { return e*e; });

    return 0;
}

template<typename T, typename P>
auto min(const T& a, const P& b) -> decltype ((a < b) ? a : b) {
    return  (a < b) ? a : b;
}
