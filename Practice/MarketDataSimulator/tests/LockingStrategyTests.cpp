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

TEST_F(LockingStrategyTests, SynchronizedQueueTwoProducersTwoConsumersCAS) {
    SynchronizedQueue<CASLockingStrategy> queue;
    std::atomic<int> pushCount{0};
    std::atomic<int> popCount{0};
    std::atomic<int> producersFinished{0};
    std::vector<int> consumedValues;  // To track what was consumed
    std::mutex consumedMutex;  // To protect the consumedValues vector
    
    const int messagesPerProducer = 1000;
    const int totalMessages = messagesPerProducer * 2;  // 2 producers
    
    // Producer 1
    auto producer1 = [&queue, &pushCount, &producersFinished, messagesPerProducer]() {
        for (int i = 0; i < messagesPerProducer; ++i) {
            MarketData data("PROD1", i * 1.0, i, std::chrono::system_clock::now(), i);
            queue.push(data);
            pushCount++;
            std::this_thread::sleep_for(std::chrono::microseconds(100));  // Small delay
        }
        producersFinished++;
    };
    
    // Producer 2
    auto producer2 = [&queue, &pushCount, &producersFinished, messagesPerProducer]() {
        for (int i = 0; i < messagesPerProducer; ++i) {
            MarketData data("PROD2", (i + 1000) * 1.0, i + 1000, std::chrono::system_clock::now(), i + 1000);
            queue.push(data);
            pushCount++;
            std::this_thread::sleep_for(std::chrono::microseconds(100));  // Small delay
        }
        producersFinished++;
    };
    
    // Consumer 1
    auto consumer1 = [&queue, &popCount, &producersFinished, &consumedValues, &consumedMutex, totalMessages]() {
        while (popCount < totalMessages) {
            MarketData data;
            if (queue.try_pop(data)) {
                popCount++;
                {
                    std::lock_guard<std::mutex> lock(consumedMutex);
                    consumedValues.push_back(data.feed_id);
                }
            } else {
                // If no data and all producers are finished, break
                if (producersFinished >= 2 && queue.empty()) {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        }
    };
    
    // Consumer 2
    auto consumer2 = [&queue, &popCount, &producersFinished, &consumedValues, &consumedMutex, totalMessages]() {
        while (popCount < totalMessages) {
            MarketData data;
            if (queue.try_pop(data)) {
                popCount++;
                {
                    std::lock_guard<std::mutex> lock(consumedMutex);
                    consumedValues.push_back(data.feed_id);
                }
            } else {
                // If no data and all producers are finished, break
                if (producersFinished >= 2 && queue.empty()) {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        }
    };
    
    // Start all threads
    std::thread p1(producer1);
    std::thread p2(producer2);
    std::thread c1(consumer1);
    std::thread c2(consumer2);
    
    // Wait for all threads to complete
    p1.join();
    p2.join();
    
    // Shutdown the queue after all producers finish
    queue.shutdown_queue();
    
    c1.join();
    c2.join();
    
    // Verify results
    EXPECT_EQ(pushCount, totalMessages);
    EXPECT_EQ(popCount, totalMessages);
    EXPECT_EQ(consumedValues.size(), totalMessages);
    
    // Verify that all sequence numbers are present (0-999 from prod1, 1000-1999 from prod2)
    std::sort(consumedValues.begin(), consumedValues.end());
    for (int i = 0; i < totalMessages; ++i) {
        EXPECT_EQ(consumedValues[i], i);
    }
    
    // Verify queue is empty after all processing
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
    
    // Print some statistics
    std::cout << "\n--- Two Producers, Two Consumers CAS Test Results ---" << std::endl;
    std::cout << "Total messages pushed: " << pushCount << std::endl;
    std::cout << "Total messages consumed: " << popCount << std::endl;
    std::cout << "Unique consumed values: " << consumedValues.size() << std::endl;
    
    // Verify statistics
    auto stats = queue.get_statistics();
    EXPECT_EQ(stats.size(), 2);  // Should have stats for both PROD1 and PROD2
    EXPECT_TRUE(stats.find("PROD1") != stats.end());
    EXPECT_TRUE(stats.find("PROD2") != stats.end());
    EXPECT_EQ(stats["PROD1"].messages_sent, messagesPerProducer);
    EXPECT_EQ(stats["PROD2"].messages_sent, messagesPerProducer);
} 
