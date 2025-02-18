#include <sqlite3.h>
#include <iostream>


#include "dbquery.h"

dbquery::dbquery(const char* dbFilePath){
    if (sqlite3_open(dbFilePath, &db) == SQLITE_OK) {
        const char* queryDataSQL = "SELECT * FROM texts;";
        if (sqlite3_prepare_v2(db, queryDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
            isOK=true;
        }
        else isOK=false;
    }
    else{
        isOK=false;
    }
}

//query id for next card in database,-1 for end.
int32_t dbquery::nextID(){
    if (sqlite3_step(statement) == SQLITE_ROW){
        return sqlite3_column_int(statement, 0);
    }
    else return -1;
}

dbquery::~dbquery(){
    sqlite3_finalize(statement);
    sqlite3_close(db);
}

