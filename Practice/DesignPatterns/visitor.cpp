#include <iostream>
#include <variant>

// Base class
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

// Derived class 1: Circle
class Circle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a Circle\n";
    }
};

// Derived class 2: Rectangle
class Rectangle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a Rectangle\n";
    }
};

// Derived class 3: Triangle
class Triangle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a Triangle\n";
    }
};


class CircleDrawer {
public:
    void draw(const Circle& c) {
        std::cout << "Drawing a Circle" << std::endl;
    }
};

class RectangleDrawer {
public:
    void draw(const Rectangle& c) {
        std::cout << "Drawing a Rectangle" << std::endl;
    }
};

class TriangleDrawer {
public:
    void draw(const Triangle& c) {
        std::cout << "Drawing a Triangle" << std::endl;
    }
};


template<typename ...Types>
class StaticCastDrawer : public Types...
{
public:
    template<typename T>
    void operator()(const T& shape) {
        if constexpr (std::is_same_v<T, Circle>) {
            static_cast<CircleDrawer*>(this)->draw(shape);
        } else if constexpr (std::is_same_v<T, Rectangle>) {
            static_cast<RectangleDrawer*>(this)->draw(shape);
        } else if constexpr (std::is_same_v<T, Triangle>) {
            static_cast<TriangleDrawer*>(this)->draw(shape);
        }
    }
};

template<typename ...Types>
class Drawer : public Types...
{
public:
    using Types::draw...;
};


// Helper to collect call operators from lambdas
template<class... Ts>
struct overload : Ts...
{
    // Make base class operators available
    using Ts::operator()...;
};

// deduction guide
// The deduction guide, which tells CTAD how to instantiate the class template overload.
// NOT required in c++ 20 - it can deduce types from ctor
// template<class... Ts>
// overload(Ts...) -> overload<Ts...>;

int _main()
{
    std::variant<Circle, Rectangle, Triangle> c = Circle();
    std::variant<Circle, Rectangle, Triangle> r = Rectangle();
    std::variant<Circle, Rectangle, Triangle> t = Triangle();

    // Option 1
    auto visitor = overload {
        [](const Circle&){ std::cout << "Drawing a Circle" << std::endl; },
            [](const Rectangle&){ std::cout << "Drawing a Rectangle" << std::endl; },
            [](const Triangle&){ std::cout << "Drawing a Triangle" << std::endl; }
    };

    std::visit(visitor, c);
    std::visit(visitor, r);

    // Option 2
    Circle c2;
    Rectangle r2;
    Triangle t2;

    StaticCastDrawer<CircleDrawer, RectangleDrawer, TriangleDrawer> sc_drawer;
    sc_drawer(r2);
    sc_drawer(t2);

    // Option 3
    Drawer<CircleDrawer, RectangleDrawer, TriangleDrawer> drawer;
    drawer.draw(r2);

    return 0;
}

