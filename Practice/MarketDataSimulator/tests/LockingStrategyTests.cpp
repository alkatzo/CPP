#include "LockingStrategyTests.h"
#include "../SynchronizedQueue.h"
#include "../MarketData.h"
#include <chrono>
#include <thread>

// Mutex locking strategy tests
TEST_F(LockingStrategyTests, MutexBasicLockUnlock) {
    MutexLockingStrategy strategy;
    
    // Test basic lock/unlock
    strategy.lock();
    strategy.unlock();
    
    // Test try_lock when not locked
    bool tryResult = strategy.try_lock();
    EXPECT_TRUE(tryResult);
    
    // Test try_lock when already locked
    bool tryResult2 = strategy.try_lock();
    EXPECT_FALSE(tryResult2);  // Should fail
    
    strategy.unlock();
}

TEST_F(LockingStrategyTests, MutexTryLock) {
    MutexLockingStrategy strategy;
    
    // Should succeed when not locked
    EXPECT_TRUE(strategy.try_lock());
    
    // Should fail when already locked
    EXPECT_FALSE(strategy.try_lock());
    
    strategy.unlock();
}

TEST_F(LockingStrategyTests, MutexMultiThreaded) {
    MutexLockingStrategy strategy;
    std::atomic<int> counter{0};
    
    auto worker = [&strategy, &counter]() {
        for (int i = 0; i < 1000; ++i) {
            strategy.lock();
            int oldVal = counter.load();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));  // Simulate some work
            counter.store(oldVal + 1);
            strategy.unlock();
        }
    };
    
    std::thread t1(worker);
    std::thread t2(worker);
    std::thread t3(worker);
    
    t1.join();
    t2.join();
    t3.join();
    
    EXPECT_EQ(counter, 3000);  // Should be exactly 3000
}

TEST_F(LockingStrategyTests, MutexConditionVariable) {
    MutexLockingStrategy strategy;
    std::atomic<bool> flag{false};
    
    auto waiter = [&strategy, &flag]() {
        std::unique_lock<std::mutex> lock(strategy.get_mutex());
        strategy.get_condition().wait(lock, [&flag] { return flag.load(); });
    };
    
    std::thread t(waiter);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    flag = true;
    strategy.get_condition().notify_one();
    
    t.join();
}

// CAS locking strategy tests
TEST_F(LockingStrategyTests, CASBasicLockUnlock) {
    CASLockingStrategy strategy;
    
    // Test basic lock/unlock
    strategy.lock();
    strategy.unlock();
    
    // Test try_lock when not locked
    bool tryResult = strategy.try_lock();
    EXPECT_TRUE(tryResult);
    
    // Test try_lock when already locked
    bool tryResult2 = strategy.try_lock();
    EXPECT_FALSE(tryResult2);  // Should fail
    
    strategy.unlock();
}

TEST_F(LockingStrategyTests, CASTryLock) {
    CASLockingStrategy strategy;
    
    // Should succeed when not locked
    EXPECT_TRUE(strategy.try_lock());
    
    // Should fail when already locked
    EXPECT_FALSE(strategy.try_lock());
    
    strategy.unlock();
}

TEST_F(LockingStrategyTests, CASMultiThreaded) {
    CASLockingStrategy strategy;
    std::atomic<int> counter{0};
    
    auto worker = [&strategy, &counter]() {
        for (int i = 0; i < 1000; ++i) {
            strategy.lock();
            int oldVal = counter.load();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));  // Simulate some work
            counter.store(oldVal + 1);
            strategy.unlock();
        }
    };
    
    std::thread t1(worker);
    std::thread t2(worker);
    std::thread t3(worker);
    
    t1.join();
    t2.join();
    t3.join();
    
    EXPECT_EQ(counter, 3000);  // Should be exactly 3000
}

// No locking strategy tests
TEST_F(LockingStrategyTests, NoLockBasic) {
    NoLockingStrategy strategy;
    
    // All operations should succeed immediately
    strategy.lock();
    EXPECT_TRUE(strategy.try_lock());  // Should always succeed
    strategy.unlock();
    strategy.unlock();
}

// SynchronizedQueue tests
TEST_F(LockingStrategyTests, SynchronizedQueueMutex) {
    SynchronizedQueue<MutexLockingStrategy> queue;
    
    // Test basic push/pop
    MarketData data1("TEST", 100.0, 100, std::chrono::system_clock::now(), 1);
    MarketData data2("TEST", 200.0, 200, std::chrono::system_clock::now(), 2);
    
    queue.push(data1);
    queue.push(data2);
    
    EXPECT_EQ(queue.size(), 2);
    
    MarketData popped1, popped2;
    EXPECT_TRUE(queue.try_pop(popped1));
    EXPECT_TRUE(queue.try_pop(popped2));
    
    EXPECT_EQ(queue.size(), 0);
}

TEST_F(LockingStrategyTests, SynchronizedQueueCAS) {
    SynchronizedQueue<CASLockingStrategy> queue;
    
    // Test basic push/pop
    MarketData data1("TEST", 100.0, 100, std::chrono::system_clock::now(), 1);
    MarketData data2("TEST", 200.0, 200, std::chrono::system_clock::now(), 2);
    
    queue.push(data1);
    queue.push(data2);
    
    EXPECT_EQ(queue.size(), 2);
    
    MarketData popped1, popped2;
    EXPECT_TRUE(queue.try_pop(popped1));
    EXPECT_TRUE(queue.try_pop(popped2));
    
    EXPECT_EQ(queue.size(), 0);
}

TEST_F(LockingStrategyTests, SynchronizedQueueNoLock) {
    SynchronizedQueue<NoLockingStrategy> queue;
    
    // Test basic push/pop
    MarketData data1("TEST", 100.0, 100, std::chrono::system_clock::now(), 1);
    MarketData data2("TEST", 200.0, 200, std::chrono::system_clock::now(), 2);
    
    queue.push(data1);
    queue.push(data2);
    
    EXPECT_EQ(queue.size(), 2);
    
    MarketData popped1, popped2;
    EXPECT_TRUE(queue.try_pop(popped1));
    EXPECT_TRUE(queue.try_pop(popped2));
    
    EXPECT_EQ(queue.size(), 0);
}

TEST_F(LockingStrategyTests, SynchronizedQueueMultiThreaded) {
    SynchronizedQueue<MutexLockingStrategy> queue;
    std::atomic<int> pushCount{0};
    std::atomic<int> popCount{0};
    std::atomic<bool> stop{false};
    
    // Producer thread
    auto producer = [&queue, &pushCount, &stop]() {
        for (int i = 0; i < 1000 && !stop; ++i) {
            MarketData data("TEST", i * 1.0, i, std::chrono::system_clock::now(), i);
            queue.push(data);
            pushCount++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };
    
    // Consumer thread
    auto consumer = [&queue, &popCount, &stop]() {
        while (!stop || !queue.empty()) {
            MarketData data;
            if (queue.try_pop(data)) {
                popCount++;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    };
    
    std::thread t1(producer);
    std::thread t2(consumer);
    
    t1.join();
    stop = true;
    t2.join();
    
    EXPECT_EQ(pushCount, popCount);  // All pushed items should be popped
} 