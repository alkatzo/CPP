#include <atomic>
#include <format>
#include <iostream>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>

struct Element {
    size_t i;
    int val() { return i; }
};
std::ostream& operator<<(std::ostream& o, const Element& e) {
    o << e.i;
    return o;
}

template <typename T>
struct element_factory {
    static T make(size_t i) { return T{i}; }
};

template <typename T>
struct element_factory<std::unique_ptr<T>> {
    static std::unique_ptr<T> make(size_t i) { return std::make_unique<T>(i); }
};

template <typename T>
T make_element(size_t i) {
    return element_factory<T>::make(i);
}

template <typename T, size_t N>
class Producer {
private:
    std::atomic<int> i{0};
    T buffer[N];

public:
    explicit Producer() {}

    void produce() {
        destroy();
        for (size_t k = 0; k < N; ++k) {
            new (buffer + k) T(k);
            i.store(k + 1, std::memory_order_release);
            std::cout << std::format("producer : {}\n", buffer[k].val());
        }
    }

    T* data() { return buffer; }
    const std::atomic<int>& counter() { return i; }

private:
    void destroy() {
        for (size_t k = 0; k < N; ++k) {
            buffer[k].~T();
        }
    }
};

template <typename T>
class Consumer {
private:
    const std::atomic<int>& i;
    T* buffer;

public:
    Consumer(T* buffer, const std::atomic<int>& i) : buffer{buffer}, i{i} {}
    void consume(size_t N) {
        for (size_t k = 0; k < N; ++k) {
            while (i.load(std::memory_order_relaxed) <= k) {
                SYS_pause;
            }
            i.load(std::memory_order_acquire);
            std::cout << std::format("consumer : {}\n", buffer[k].val());
        }
    }
};

template <typename T>
constexpr T half(T x) {
    return x / 2;
}

template <typename T>
constexpr T double_f(T x);

template <typename T1, typename T2>
constexpr std::common_type_t<T1, T2> Max(T1 x, T2 y) {
    return (x > y) ? x : y;
}

// SFINAE example for a type trait
template <typename T, typename U>
struct try_common {
private:
    template <typename X, typename Y>
    static auto test(int) -> decltype(true ? std::declval<X>() : std::declval<Y>());

    template <typename, typename>
    static void test(...);

public:
    using type = decltype(test<T, U>(0));
};

int main(int argc, char** argv) {
    constexpr auto N = half(20);
    constexpr decltype(double_f(1)) M = 1;
    constexpr auto O = Max(2L, 3);

    try_common<int, double>::type a;  // ok, type = double
    // fails, uses fallback → usually void. "void b;" is illegal
    // try_common<Consumer<int>, Producer<int, 10>>::type b;

    static_assert(N == 10, "failed");

    struct alignas(64) Stats {
        uint64_t a;
        uint64_t b;
    };

    std::cout << "sizeof(Stats): " << sizeof(Stats) << std::endl;

    using E = Element;
    std::atomic<int> counter{0};
    Producer<E, N> p;
    Consumer<E> c(p.data(), p.counter());

    std::jthread ct(&Consumer<E>::consume, &c, N);
    std::jthread pt(&Producer<E, N>::produce, &p);

    return 0;
}