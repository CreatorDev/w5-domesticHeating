//
//  SQLiteStorage.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 02/01/17.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "SQLiteStorage.hpp"
#include <sqlite3.h>

SQLiteStorage::SQLiteStorage()
: db(nullptr),
  temperatureSerializer(nullptr),
  movementSerializer(nullptr),
  systemEventSerializer(nullptr) {
}

SQLiteStorage::~SQLiteStorage() {
    close();
}

bool SQLiteStorage::open(const string& dbPath) {
  int rc = sqlite3_open(dbPath.c_str(), &db);
  if ( rc ){
    //todo: proper logs not fprint
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    db = NULL;
    return false;
  }
  temperatureSerializer = make_shared<SQLiteTemperatureSensorValueSerializer>(db, "REAL", "temperatures");
  movementSerializer = make_shared<SQLiteMovementSensorValueSerializer>(db, "INT", "movements");
  systemEventSerializer = make_shared<SQLiteSystemEventValueSerializer>(db, "INT", "system_events");
  return true;
}

void SQLiteStorage::close() {
  //todo: proper logs
  if (db != NULL) {
    sqlite3_close(db);
    db = NULL;
    temperatureSerializer = nullptr;
    movementSerializer = nullptr;
    systemEventSerializer = nullptr;
  }
}

shared_ptr<SQLiteTemperatureSensorValueSerializer> SQLiteStorage::getTemperatureSerializer() {
  return temperatureSerializer;
}

shared_ptr<SQLiteMovementSensorValueSerializer> SQLiteStorage::getMovementSerializer() {
  return movementSerializer;
}

shared_ptr<SQLiteSystemEventValueSerializer> SQLiteStorage::getSystemEventSerializer() {
  return systemEventSerializer;
}
