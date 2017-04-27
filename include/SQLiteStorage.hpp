//
//  SQLiteStorage.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 02/01/17.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLiteStorage_hpp
#define SQLiteStorage_hpp

#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include "SQLiteSensorValueSerializer.hpp"

using namespace std;

class SQLiteStorage {
  public:
    SQLiteStorage();
    ~SQLiteStorage();
  
    bool open(const string& dbPath);
    void close();
    shared_ptr<SQLiteTemperatureSensorValueSerializer> getTemperatureSerializer();
    shared_ptr<SQLiteMovementSensorValueSerializer> getMovementSerializer();
    shared_ptr<SQLiteSystemEventValueSerializer> getSystemEventSerializer();
  protected:
    sqlite3*  db;
    shared_ptr<SQLiteTemperatureSensorValueSerializer>  temperatureSerializer;
    shared_ptr<SQLiteMovementSensorValueSerializer>     movementSerializer;
    shared_ptr<SQLiteSystemEventValueSerializer>        systemEventSerializer;
};

#endif /* SQLiteStorage_hpp */
