#include <iostream>
#include <future>
#include <any>
#include <optional>
#include <variant>

void use_any() {
    std::any a = 42;
    a = std::string("hello");

    try {
        std::string s = std::any_cast<std::string>(a);
        std::cout << s << '\n';
    } catch (const std::bad_any_cast& e) {
        std::cout << "Bad any_cast: " << e.what() << '\n';
    }
}

std::optional<int> find_even(int x) {
    if (x % 2 == 0) return x;
    return std::nullopt;
}

void use_variant() {
    std::variant<int, std::string> v;
    v = 42;
    v = std::string("hello");

    std::visit([](auto&& arg) {
        std::cout << "Value: " << arg << '\n';
    }, v);

    if(std::holds_alternative<std::string>(v)) {
        std::cout << "It's a string: " << std::get<std::string>(v) << '\n';
    }
}

int main()
{    
    auto result = find_even(4);
    if (result) {
        std::cout << "Even: " << *result << '\n';
    } else {
        std::cout << "Not even\n";
    }

    use_variant();

    // thread with future / promise example
    // Worker function
    {
        auto l = [](int x, std::promise<int> resultPromise) {
            std::this_thread::sleep_for(std::chrono::seconds(1)); // simulate delay
            int result = x * x;
            resultPromise.set_value(result); // send result back
        };
        std::promise<int> promise;
        std::future<int> fut = promise.get_future();

        std::thread worker(l, 7, std::move(promise));
        std::cout << "7^2 = " << fut.get() << std::endl;
        // worker.detach();
        worker.join();

        std::cout << "finished" << std::endl;
    }

    return 0;
}
