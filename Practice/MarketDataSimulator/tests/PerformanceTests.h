#pragma once

#include "../LockingStrategy.h"
#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include <iomanip>

class PerformanceTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
    
    // Template method declarations
    template<typename LockStrategy>
    double measureLockingStrategyPerformance(int iterations, int numThreads);
    
    template<typename LockStrategy>
    double measureQueuePerformance(int iterations, int numProducers, int numConsumers);
}; 