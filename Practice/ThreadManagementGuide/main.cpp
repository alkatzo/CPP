#include <iostream>
#include <thread>
#include <chrono>
#include <queue>

using namespace std;


class cleaning_crew {
public:
    void operator()() {
        std::cout << __FUNCTION__ << std::endl;
    }
};

class crew {
public:
    void operator()() {
        std::cout << __FUNCTION__ << std::endl;
    }
};


class thread_guard {

    std::thread& t;

public:
    explicit thread_guard(std::thread& t) : t(t)
    {}


    ~thread_guard() {
        if(t.joinable()) {
            t.join();
        }
    }

    thread_guard(const thread_guard &) = delete;
    thread_guard & operator=(const thread_guard &) = delete;

};

int main()
{    
    std::cout << "Allowed threads " << std::thread::hardware_concurrency() << std::endl;


    std::queue<int> clean_crew;
    std::queue<int> engine_crew;

    std::thread processor([&clean_crew, &engine_crew]() {
        int i = -1;
        while(true) {
            std::cin >> i;

            switch(i) {
            case 1: {
                clean_crew.push(1);
                break;
            }
            case 2: {
                engine_crew.push(1);
                break;
            }
            default:
                return 0;
            }
        }
    });


    std::thread clean_processor([&clean_crew](){
        while(true) {
            while(clean_crew.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            }
            clean_crew.pop();
            std::cout << "Cleaning..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });

    std::thread engine_processor([&engine_crew](){
        while(true) {
            while(engine_crew.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            }
            engine_crew.pop();
            std::cout << "Startign engine..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });


    clean_processor.detach();
    engine_processor.detach();
    processor.join();

    return 0;
}
