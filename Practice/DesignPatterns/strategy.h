#pragma once
#include <iostream>
#include <memory>

class Strategy
{
public:
    virtual void print() = 0;
};

class FastStrategy : public Strategy {
    virtual void print() override {
        std::cout << "Fast";
    }
};

class SlowStrategy : public Strategy {
    virtual void print() override {
        std::cout << "Slow";
    }
};

class StrategyOwner {
public:
    StrategyOwner(std::unique_ptr<Strategy> strategy) : strategy{std::move(strategy)} {}
    void execute() {
        strategy->print();
    }

private:
    std::unique_ptr<Strategy> strategy;
};

void useStrategy() {
    StrategyOwner so(std::make_unique<FastStrategy>());
    so.execute();
}
