#include <gtest/gtest.h>
#include "LockingStrategyTests.h"
#include "PerformanceTests.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "=== Locking Strategy Unit Tests ===" << std::endl;
    std::cout << "Testing MutexLockingStrategy, CASLockingStrategy, and NoLockingStrategy" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    return RUN_ALL_TESTS();
} 