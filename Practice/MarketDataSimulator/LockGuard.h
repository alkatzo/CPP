#pragma once

#include <memory>

// Template lock guard that works with any locking strategy
template<typename LockStrategy>
class LockGuard {
private:
    const LockStrategy& lock_strategy;

public:
    explicit LockGuard(const LockStrategy& strategy) : lock_strategy(strategy) {
        lock_strategy.lock();
    }
    
    ~LockGuard() {
        lock_strategy.unlock();
    }
    
    // Delete copy and move operations
    LockGuard(const LockGuard&) = delete;
    LockGuard(LockGuard&&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
    LockGuard& operator=(LockGuard&&) = delete;
};

// Template unique lock that works with any locking strategy
template<typename LockStrategy>
class UniqueLock {
private:
    const LockStrategy& lock_strategy;
    bool owns_lock_;

public:
    explicit UniqueLock(const LockStrategy& strategy) : lock_strategy(strategy), owns_lock_(false) {}
    
    ~UniqueLock() {
        if (owns_lock_) {
            lock_strategy.unlock();
        }
    }
    
    // Delete copy operations
    UniqueLock(const UniqueLock&) = delete;
    UniqueLock& operator=(const UniqueLock&) = delete;
    
    // Allow move operations
    UniqueLock(UniqueLock&& other) noexcept 
        : lock_strategy(other.lock_strategy), owns_lock_(other.owns_lock_) {
        other.owns_lock_ = false;
    }
    
    UniqueLock& operator=(UniqueLock&& other) noexcept {
        if (this != &other) {
            if (owns_lock_) {
                lock_strategy.unlock();
            }
            owns_lock_ = other.owns_lock_;
            other.owns_lock_ = false;
        }
        return *this;
    }
    
    void lock() {
        if (!owns_lock_) {
            lock_strategy.lock();
            owns_lock_ = true;
        }
    }
    
    void unlock() {
        if (owns_lock_) {
            lock_strategy.unlock();
            owns_lock_ = false;
        }
    }
    
    bool owns_lock() const { return owns_lock_; }
    
    // Template method for condition variable wait
    template<typename Predicate>
    void wait(std::condition_variable& cv, Predicate pred) {
        cv.wait(*this, pred);
    }
    
    // Conversion operator for condition variable
    operator std::unique_lock<std::mutex>() {
        // This is a simplified version - in practice, you'd need to adapt this
        // based on the actual locking strategy implementation
        return std::unique_lock<std::mutex>();
    }
}; 