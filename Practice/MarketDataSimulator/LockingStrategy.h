#pragma once

#include <mutex>
#include <atomic>
#include <condition_variable>

// Base locking strategy interface
template<typename T>
class LockingStrategy {
public:
    virtual ~LockingStrategy() = default;
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual bool try_lock() = 0;
};

// Mutex-based locking strategy
class MutexLockingStrategy {
private:
    mutable std::mutex mutex;
    std::condition_variable condition;

public:
    // Constructors
    MutexLockingStrategy() = default;
    MutexLockingStrategy(const MutexLockingStrategy&) = delete;
    MutexLockingStrategy(MutexLockingStrategy&&) = delete;
    
    // Assignment operators
    MutexLockingStrategy& operator=(const MutexLockingStrategy&) = delete;
    MutexLockingStrategy& operator=(MutexLockingStrategy&&) = delete;
    
    // Destructor
    ~MutexLockingStrategy() = default;
    
    void lock() const { mutex.lock(); }
    void unlock() const { mutex.unlock(); }
    bool try_lock() const { return mutex.try_lock(); }
    
    // Condition variable access
    std::condition_variable& get_condition() { return condition; }
    const std::condition_variable& get_condition() const { return condition; }
    
    // Get underlying mutex for condition variable
    std::mutex& get_mutex() { return mutex; }
    const std::mutex& get_mutex() const { return mutex; }
};

// CAS-based locking strategy (spinlock)
class CASLockingStrategy {
private:
    mutable std::atomic<bool> locked{false};
    std::condition_variable condition;

public:
    // Constructors
    CASLockingStrategy() = default;
    CASLockingStrategy(const CASLockingStrategy&) = delete;
    CASLockingStrategy(CASLockingStrategy&&) = delete;
    
    // Assignment operators
    CASLockingStrategy& operator=(const CASLockingStrategy&) = delete;
    CASLockingStrategy& operator=(CASLockingStrategy&&) = delete;
    
    // Destructor
    ~CASLockingStrategy() = default;
    
    void lock() const {
        bool expected = false;
        while (!locked.compare_exchange_weak(expected, true, std::memory_order_acquire)) {
            expected = false;
        }
    }
    
    void unlock() const {
        locked.store(false, std::memory_order_release);
    }
    
    bool try_lock() const {
        bool expected = false;
        return locked.compare_exchange_strong(expected, true, std::memory_order_acquire);
    }
    
    // Condition variable access (note: CAS doesn't work well with condition variables)
    std::condition_variable& get_condition() { return condition; }
    const std::condition_variable& get_condition() const { return condition; }
};

// No-lock strategy (for single-threaded use or when external synchronization is used)
class NoLockingStrategy {
private:
    std::condition_variable condition;

public:
    // Constructors
    NoLockingStrategy() = default;
    NoLockingStrategy(const NoLockingStrategy&) = default;
    NoLockingStrategy(NoLockingStrategy&&) = default;
    
    // Assignment operators
    NoLockingStrategy& operator=(const NoLockingStrategy&) = default;
    NoLockingStrategy& operator=(NoLockingStrategy&&) = default;
    
    // Destructor
    ~NoLockingStrategy() = default;
    
    void lock() const { /* No-op */ }
    void unlock() const { /* No-op */ }
    bool try_lock() const { return true; }
    
    // Condition variable access
    std::condition_variable& get_condition() { return condition; }
    const std::condition_variable& get_condition() const { return condition; }
}; 