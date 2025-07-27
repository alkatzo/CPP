# Locking Strategy Tests

This directory contains comprehensive unit tests and performance benchmarks for the different locking strategies implemented in the Market Data Simulator.

## Test Structure

### Unit Tests (`LockingStrategyTests.h`)
Tests the correctness and thread safety of each locking strategy:

#### MutexLockingStrategy Tests
- **Basic Lock/Unlock**: Verifies basic locking functionality
- **Try Lock**: Tests non-blocking lock attempts
- **Multi-threaded Safety**: Ensures thread safety with concurrent access
- **Condition Variable**: Tests integration with condition variables

#### CASLockingStrategy Tests
- **Basic Lock/Unlock**: Verifies CAS-based spinlock functionality
- **Try Lock**: Tests non-blocking CAS lock attempts
- **Multi-threaded Safety**: Ensures thread safety with concurrent access

#### NoLockingStrategy Tests
- **Basic Operations**: Verifies no-op behavior for single-threaded scenarios

#### SynchronizedQueue Tests
- **Basic Operations**: Tests push/pop operations with each strategy
- **Multi-threaded**: Tests producer-consumer scenarios

### Performance Tests (`PerformanceTests.h`)
Benchmarks the performance characteristics of each locking strategy:

#### Locking Strategy Performance
- Measures lock/unlock overhead for each strategy
- Compares performance ratios between strategies
- Uses 1M iterations with 4 threads

#### SynchronizedQueue Performance
- Measures producer-consumer throughput
- Tests with 2 producers and 2 consumers
- Uses 100K iterations

## Running the Tests

### Build and Run
```bash
# Build the project
mkdir build && cd build
cmake ..
cmake --build .

# Run unit tests
./locking_strategy_tests

# Or use the test target
cmake --build . --target test
```

### Expected Output
```
=== Locking Strategy Unit Tests ===
Testing MutexLockingStrategy, CASLockingStrategy, and NoLockingStrategy
=====================================
Running 13 tests...

Test: MutexLockingStrategy - Basic Lock/Unlock ... PASSED
Test: MutexLockingStrategy - Try Lock ... PASSED
Test: MutexLockingStrategy - Multi-threaded Safety ... PASSED
Test: MutexLockingStrategy - Condition Variable ... PASSED
Test: CASLockingStrategy - Basic Lock/Unlock ... PASSED
Test: CASLockingStrategy - Try Lock ... PASSED
Test: CASLockingStrategy - Multi-threaded Safety ... PASSED
Test: NoLockingStrategy - Basic Operations ... PASSED
Test: SynchronizedQueue<MutexLockingStrategy> - Basic Operations ... PASSED
Test: SynchronizedQueue<CASLockingStrategy> - Basic Operations ... PASSED
Test: SynchronizedQueue<NoLockingStrategy> - Basic Operations ... PASSED
Test: SynchronizedQueue<MutexLockingStrategy> - Multi-threaded ... PASSED

Test Results: 12 passed, 0 failed

🎉 All tests passed!

=== Performance Tests ===
Comparing different locking strategies...

--- Locking Strategy Performance ---
MutexLockingStrategy: 245.67 ms
CASLockingStrategy:   189.23 ms
NoLockingStrategy:    12.45 ms

Performance ratios (lower is better):
Mutex vs NoLock: 19.73x slower
CAS vs NoLock:   15.20x slower
CAS vs Mutex:    0.77x

--- SynchronizedQueue Performance ---
MutexLockingStrategy: 156.78 ms
CASLockingStrategy:   134.56 ms
NoLockingStrategy:    23.45 ms

Performance ratios (lower is better):
Mutex vs NoLock: 6.69x slower
CAS vs NoLock:   5.74x slower
CAS vs Mutex:    0.86x
```

## Test Framework

The tests use a simple custom test framework (`TestFramework.h`) that provides:
- Test registration and execution
- Pass/fail reporting
- Exception handling
- Utility functions for timing and waiting

## Performance Characteristics

### Expected Results
- **NoLockingStrategy**: Fastest (baseline)
- **CASLockingStrategy**: Good for low contention
- **MutexLockingStrategy**: Best for high contention

### When to Use Each Strategy
- **NoLockingStrategy**: Single-threaded applications or when external synchronization is used
- **CASLockingStrategy**: Low contention scenarios, short critical sections
- **MutexLockingStrategy**: High contention scenarios, long critical sections, when condition variables are needed

## Adding New Tests

To add new tests:

1. Add test function to `LockingStrategyTests` class
2. Register the test in `runAllTests()` method
3. Follow the naming convention: `test[Strategy][TestName]`

Example:
```cpp
static bool testNewFeature() {
    // Test implementation
    return true; // or false if test fails
}

// In runAllTests():
framework.addTest("New Feature Test", testNewFeature);
``` 