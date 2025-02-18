#ifndef DBQ_H
#define DBQ_H

#include <sqlite3.h>
#include <iostream>
#include <cstdint>

class dbquery
{
private:
    sqlite3* db;
    sqlite3_stmt* statement;
public:
    bool isOK; //true for open database success
    int32_t nextID();

    dbquery(const char* dbFilePath);
    ~dbquery();
};
#endif