#include <cstring>
#include <memory>
#include <utility>

class Buffer {
public:
    Buffer(size_t size) : data(std::make_unique<char[]>(size)) {
    }

private:
    std::unique_ptr<char[]> data;
};


class Test {
public:
    explicit Test(int* pi, int* pj = nullptr) : pi{pi}, pj{pj} {}

    ~Test() {
        delete pi;
        delete pj;
    }

    Test(const Test& o) {
        pi = o.pi ? new int{*o.pi} : nullptr;
        pj = o.pj ? new int{*o.pj} : nullptr;
    }

    Test& operator=(const Test& o) {
        if(this != &o) {
            Test tmp{o};
            swap(tmp);
        }

        return *this;
    }

    Test(Test&& o) {
        swap(o);
    }

    Test& operator=(Test&& o) {
        if(this != &o) {
            delete pi;
            delete pj;

            pi = std::exchange(o.pi, nullptr);
            pj = std::exchange(o.pj, nullptr);
        }

        return *this;
    }

private:
    void swap(Test& o) {
        int* tmp = pi;
        pi = o.pi;
        o.pi = tmp;
    }

private:
    int* pi{};
    int* pj{};
};

/**
 * concepts
 */

template<typename T>
concept Numeric = requires(T a){
    a + 1; a * 1;
};

// single constraint
auto arg42(const Numeric auto& arg) {
    return arg + 42;
}

// multiple constraint
template<typename T>
requires Numeric<T> && std::is_integral_v<T>
auto arg43(const T& arg) {
    return arg + 43;
}


int main(int argc, char** argv) {
    Buffer b1(2);
    // Buffer b2(b1); // Compiler error because data is non-copiable. In this case compiler deletes the copy ctor
    Buffer b2(std::move(b1));

    auto r = arg42(2);
    r = arg43(3);

    return 0;
}

