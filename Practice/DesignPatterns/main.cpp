#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <functional>

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

class ShapeFactory {
private:
    std::unordered_map<std::type_index, std::function<std::shared_ptr<Shape>()>> registry;

public:
    template<typename S>
    void registerShape() {
        registry[typeid(S)] = [](){
            return std::make_shared<S>();
        };;
    }

    std::shared_ptr<Shape> createShape(std::type_index tidx) {
        if(registry.contains(tidx)) {
            return registry[tidx]();
        }
        return nullptr;
    }
};


// int main()
// {

//     ShapeFactory sf;

//     sf.registerShape<Circle>();
//     sf.registerShape<Rectangle>();
//     sf.registerShape<Triangle>();


//     if(auto sp = sf.createShape(typeid(Rectangle))) {
//         sp->draw();
//     }


//     return 0;
// }

