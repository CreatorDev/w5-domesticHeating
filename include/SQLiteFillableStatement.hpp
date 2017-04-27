//
//  SQLiteFillableStatement.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 02/01/17.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLiteFillableStatement_hpp
#define SQLiteFillableStatement_hpp

#include <stdio.h>
#include <string>

#include "sqlite3.h"

using namespace std;

class SQLiteFillableStatement {
  public:
    SQLiteFillableStatement(sqlite3* db, string const & sql);
    ~SQLiteFillableStatement();
  
    bool bindNext(std::string const & value);
    bool bindNext(int const & value);
    bool bindNext(long const & value);
    bool bindNext(double const & value);
    bool executeUpdate(int* count = nullptr);
    long executeInsert();
    // <0 error, 0 - no more data, >0 row loaded
    int executeSelectNext();
    template <typename T> void getColumns(T* t) {
      getSingleColumn(t);
    }
  
    template<typename T, typename... Args> void getColumns(T* t, Args... args) {
      getSingleColumn(t);
      getColumns(args...);
    }
  private:
    int             nextBindIndex;
    int             nextColumnIndex;
    sqlite3_stmt*   statement;
    sqlite3*        db;

    void getSingleColumn(int* result);
    void getSingleColumn(long* result);
    void getSingleColumn(double* result);
    void getSingleColumn(std::string* result);
};

#endif /* SQLiteFillableStatement_hpp */
