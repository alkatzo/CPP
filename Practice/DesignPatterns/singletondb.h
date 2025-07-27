#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <limits>

namespace sng {

class DB
{
public:
    virtual int getPopulation(const std::string& name) = 0;
};

class SingletonDB : public DB
{
    SingletonDB() {
        std::ifstream is("C:\\Users\\User\\Projects\\data\\capitals.txt");

        if(!is)
            throw std::runtime_error("Cant open the file");

        std::string s, s2;

        int x;
        while(std::getline(is, s) && (is >> x)) {
            db.insert({s, x});
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::map<std::string, int> db;
public:
    SingletonDB(const SingletonDB&) = delete;
    void operator=(const SingletonDB&) = delete;

    static SingletonDB& instance() {
        static SingletonDB _instance;
        return _instance;
    }

    virtual int getPopulation(const std::string& name) override {
        return db[name];
    }
};

class TestDB : public DB
{
public:
    TestDB() {
        db["Tokyo"] = 2121312;
        db["Moscow"] = 32523;
        db["Sydney"] = 453421312;
    }

    virtual int getPopulation(const std::string& name) override {
        return db[name];
    }
    TestDB(const TestDB&) = delete;
    void operator=(const TestDB&) = delete;

private:
    std::map<std::string, int> db;
};

}
