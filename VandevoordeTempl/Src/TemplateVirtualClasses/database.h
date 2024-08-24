#ifndef DATABASE_H
#define DATABASE_H

class Database {
public:
    inline static Database *the = nullptr;

public:
    static void make_db();
    virtual void greet() = 0;

};

#endif // DATABASE_H
