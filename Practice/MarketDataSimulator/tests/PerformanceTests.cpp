#include "PerformanceTests.h"
#include "../SynchronizedQueue.h"
#include "../MarketData.h"
#include <chrono>
#include <thread>
#include <vector>

TEST_F(PerformanceTests, LockingStrategyPerformance) {
    std::cout << "\n--- Locking Strategy Performance ---" << std::endl;
    
    const int iterations = 1000000;
    const int numThreads = 4;
    
    // Test MutexLockingStrategy
    auto mutexTime = measureLockingStrategyPerformance<MutexLockingStrategy>(iterations, numThreads);
    
    // Test CASLockingStrategy
    auto casTime = measureLockingStrategyPerformance<CASLockingStrategy>(iterations, numThreads);
    
    // Test NoLockingStrategy (baseline)
    auto noLockTime = measureLockingStrategyPerformance<NoLockingStrategy>(iterations, numThreads);
    
    // Print results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "MutexLockingStrategy: " << mutexTime << " ms" << std::endl;
    std::cout << "CASLockingStrategy:   " << casTime << " ms" << std::endl;
    std::cout << "NoLockingStrategy:    " << noLockTime << " ms" << std::endl;
    
    std::cout << "\nPerformance ratios (lower is better):" << std::endl;
    std::cout << "Mutex vs NoLock: " << (mutexTime / noLockTime) << "x slower" << std::endl;
    std::cout << "CAS vs NoLock:   " << (casTime / noLockTime) << "x slower" << std::endl;
    std::cout << "CAS vs Mutex:    " << (casTime / mutexTime) << "x" << std::endl;
    
    // Basic assertions to ensure tests run
    EXPECT_GT(mutexTime, 0);
    EXPECT_GT(casTime, 0);
    EXPECT_GT(noLockTime, 0);
}

TEST_F(PerformanceTests, SynchronizedQueuePerformance) {
    std::cout << "\n--- SynchronizedQueue Performance ---" << std::endl;
    
    const int iterations = 100000;
    const int numProducers = 2;
    const int numConsumers = 2;
    
    // Test MutexLockingStrategy
    auto mutexTime = measureQueuePerformance<MutexLockingStrategy>(iterations, numProducers, numConsumers);
    
    // Test CASLockingStrategy
    auto casTime = measureQueuePerformance<CASLockingStrategy>(iterations, numProducers, numConsumers);
    
    // Test NoLockingStrategy
    auto noLockTime = measureQueuePerformance<NoLockingStrategy>(iterations, numProducers, numConsumers);
    
    // Print results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "MutexLockingStrategy: " << mutexTime << " ms" << std::endl;
    std::cout << "CASLockingStrategy:   " << casTime << " ms" << std::endl;
    std::cout << "NoLockingStrategy:    " << noLockTime << " ms" << std::endl;
    
    std::cout << "\nPerformance ratios (lower is better):" << std::endl;
    std::cout << "Mutex vs NoLock: " << (mutexTime / noLockTime) << "x slower" << std::endl;
    std::cout << "CAS vs NoLock:   " << (casTime / noLockTime) << "x slower" << std::endl;
    std::cout << "CAS vs Mutex:    " << (casTime / mutexTime) << "x" << std::endl;
    
    // Basic assertions to ensure tests run
    EXPECT_GT(mutexTime, 0);
    EXPECT_GT(casTime, 0);
    EXPECT_GT(noLockTime, 0);
}

template<typename LockStrategy>
double PerformanceTests::measureLockingStrategyPerformance(int iterations, int numThreads) {
    LockStrategy strategy;
    std::atomic<int> counter{0};
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> threads;
    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back([&strategy, &counter, iterations]() {
            for (int i = 0; i < iterations / 4; ++i) {
                strategy.lock();
                counter++;
                strategy.unlock();
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    return duration.count();
}

template<typename LockStrategy>
double PerformanceTests::measureQueuePerformance(int iterations, int numProducers, int numConsumers) {
    SynchronizedQueue<LockStrategy> queue;
    std::atomic<int> pushCount{0};
    std::atomic<int> popCount{0};
    std::atomic<bool> stop{false};
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Start producer threads
    std::vector<std::thread> producers;
    for (int p = 0; p < numProducers; ++p) {
        producers.emplace_back([&queue, &pushCount, iterations, numProducers]() {
            for (int i = 0; i < iterations / numProducers; ++i) {
                MarketData data("TEST", i * 1.0, i, std::chrono::system_clock::now(), i);
                queue.push(data);
                pushCount++;
            }
        });
    }
    
    // Start consumer threads
    std::vector<std::thread> consumers;
    for (int c = 0; c < numConsumers; ++c) {
        consumers.emplace_back([&queue, &popCount, &stop, iterations]() {
            while (popCount < iterations) {
                MarketData data;
                if (queue.try_pop(data)) {
                    popCount++;
                }
            }
        });
    }
    
    // Wait for all threads
    for (auto& thread : producers) {
        thread.join();
    }
    
    stop = true;
    for (auto& thread : consumers) {
        thread.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    return duration.count();
} 