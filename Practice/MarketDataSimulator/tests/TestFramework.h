#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>

// Simple test framework
class TestFramework {
private:
    std::vector<std::pair<std::string, std::function<bool()>>> tests;
    std::atomic<int> passed{0};
    std::atomic<int> failed{0};

public:
    void addTest(const std::string& name, std::function<bool()> testFunc) {
        tests.emplace_back(name, testFunc);
    }
    
    bool runTests() {
        std::cout << "Running " << tests.size() << " tests...\n" << std::endl;
        
        for (const auto& [name, testFunc] : tests) {
            std::cout << "Test: " << name << " ... ";
            std::cout.flush();
            
            try {
                bool result = testFunc();
                if (result) {
                    std::cout << "PASSED" << std::endl;
                    passed++;
                } else {
                    std::cout << "FAILED" << std::endl;
                    failed++;
                }
            } catch (const std::exception& e) {
                std::cout << "FAILED (Exception: " << e.what() << ")" << std::endl;
                failed++;
            } catch (...) {
                std::cout << "FAILED (Unknown exception)" << std::endl;
                failed++;
            }
        }
        
        std::cout << "\nTest Results: " << passed << " passed, " << failed << " failed" << std::endl;
        return failed == 0;
    }
    
    int getPassed() const { return passed; }
    int getFailed() const { return failed; }
};

// Test utilities
class TestUtils {
public:
    static void sleep_ms(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
    
    static bool waitForCondition(std::function<bool()> condition, int timeout_ms = 1000) {
        auto start = std::chrono::steady_clock::now();
        while (!condition()) {
            if (std::chrono::steady_clock::now() - start > std::chrono::milliseconds(timeout_ms)) {
                return false;
            }
            sleep_ms(10);
        }
        return true;
    }
}; 