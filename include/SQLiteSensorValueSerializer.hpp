//
//  SQLiteSensorValueSerializer.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 03/01/17.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLiteSensorValueSerializer_hpp
#define SQLiteSensorValueSerializer_hpp

#include <stdio.h>
#include <vector>
#include <sqlite3.h>
#include <sstream>
#include <string>

#include "SimpleCriteria.hpp"
#include "SensorValue.hpp"
#include "SQLiteFillableStatement.hpp"

template <typename T>
class SQLiteSensorValueSerializer {
  public:
    SQLiteSensorValueSerializer(sqlite3* db, string contType, string tableName);
    void store(SensorValue<T>* data);
    void storeOrUpdate(SensorValue<T>* data);
    
    shared_ptr<vector<shared_ptr<SensorValue<T>>>> loadMatching(SimpleCriteria criteria);
    shared_ptr<SensorValue<T>> load(long id);
    const string& getTableName() const;
  private:
    sqlite3*  db;
    string tableName;
  
    void append(string& sql, bool condition, const string& toAppend, bool &andMarker);
    shared_ptr<SensorValue<T>> deserializeRow(SQLiteFillableStatement& statement);
};

typedef SQLiteSensorValueSerializer<double> SQLiteTemperatureSensorValueSerializer;
typedef SQLiteSensorValueSerializer<MovementStates> SQLiteMovementSensorValueSerializer;
typedef SQLiteSensorValueSerializer<SystemEvent> SQLiteSystemEventValueSerializer;

template <typename T>
SQLiteSensorValueSerializer<T>::SQLiteSensorValueSerializer(sqlite3* _db, string contType, string tableName)
: db(_db), tableName(tableName) {
  
  stringstream stream;
  stream << "CREATE TABLE IF NOT EXISTS " << tableName <<" (";
  stream << "id INTEGER PRIMARY KEY, ";
  stream << "physicalSensorId INT NOT NULL,";
  stream << "value " << contType << " NOT NULL,";
  stream << "valueType INT NOT NULL,";
  stream << "timestamp INT NOT NULL";
  stream << ")";

  //create table if needed
  SQLiteFillableStatement statement(db, stream.str());
  statement.executeUpdate();
}

template<typename T>
const string& SQLiteSensorValueSerializer<T>::getTableName() const {
  return tableName;
}

template <typename T>
void SQLiteSensorValueSerializer<T>::store(SensorValue<T>* data) {

  stringstream stream;
  stream << "INSERT INTO "
         << getTableName()
         <<" VALUES (NULL, ?, ?, ?, ?)";

  SQLiteFillableStatement statement(db, stream.str());
  statement.bindNext(data->getPhysicalSensorId());
  statement.bindNext(data->getValue());
  statement.bindNext( static_cast<int>(data->getValueType()) );
  statement.bindNext( static_cast<long>( data->getTimestamp() ));
  data->setId( statement.executeInsert() );
}

template <typename T>
void SQLiteSensorValueSerializer<T>::storeOrUpdate(SensorValue<T>* data) {
  if (data->getId() < 0) {
    store(data);
    return;
  }
  
  stringstream stream;
  stream << "INSERT OR REPLACE INTO "
         << getTableName()
         << " (id, physicalSensorId, value, valueType, timestamp) VALUES (?, ?, ?, ?, ?)";
  SQLiteFillableStatement statement(db, stream.str());
  statement.bindNext(data->getId());
  statement.bindNext(data->getPhysicalSensorId());
  statement.bindNext(data->getValue());
  statement.bindNext( static_cast<int>(data->getValueType()) );
  statement.bindNext( static_cast<long>( data->getTimestamp() ));
  statement.executeUpdate();
}

template <typename T>
void SQLiteSensorValueSerializer<T>::append(string& sql, bool condition, const string& toAppend, bool &andMarker) {
  if (condition == false) {
    return;
  }
  if (andMarker) {
    sql.append(" AND ");
  }
  andMarker = true;
  sql.append(toAppend);
}

template <typename T>
shared_ptr<SensorValue<T>> SQLiteSensorValueSerializer<T>::deserializeRow(SQLiteFillableStatement& statement) {
  long id, sensId, timestamp;
  int valType;
  T value;
  
  statement.getColumns( &id, &sensId, &value, &valType, &timestamp);
  
  return make_shared<SensorValue<T>>(id, sensId, value, static_cast<SensorValueType>(valType), static_cast<time_t>(timestamp));
}

template <typename T>
shared_ptr<vector<shared_ptr<SensorValue<T>>>> SQLiteSensorValueSerializer<T>::loadMatching(SimpleCriteria criteria) {
  
  bool andRequired = false;
  stringstream stream;
  stream << "SELECT * FROM " << getTableName() << " WHERE ";

  string sql(stream.str());
  
  append(sql, criteria.from > 0, "(timestamp >= ?)", andRequired);
  append(sql, criteria.to > 0, "(timestamp <= ?)", andRequired);
  
  andRequired = false;
  append(sql, criteria.count > 0, " LIMIT ?", andRequired);
  
  andRequired = false;
  append(sql, criteria.offset > 0, " OFFSET ?", andRequired);
  
  //build statement
  SQLiteFillableStatement statement(db, sql);
  
  //bind data, NOTE: order is important!
  if (criteria.from > 0) {
    statement.bindNext( static_cast<long>(criteria.from) );
  }
  if (criteria.to > 0) {
    statement.bindNext( static_cast<long>(criteria.to) );
  }
  if (criteria.count > 0) {
    statement.bindNext( criteria.count );
  }
  if (criteria.offset > 0) {
    statement.bindNext( criteria.offset );
  }
  
  auto result = make_shared<vector<shared_ptr<SensorValue<T>>>>();
  
  while(statement.executeSelectNext() > 0) {
    result->push_back(deserializeRow(statement));
  }
  
  return result;
}

template <typename T>
shared_ptr<SensorValue<T>> SQLiteSensorValueSerializer<T>::load(long id) {

  stringstream stream;
  stream << "SELECT * FROM " <<getTableName() << " WHERE id=?";
  SQLiteFillableStatement statement(db, stream.str());
  statement.bindNext(id);
  if (statement.executeSelectNext() > 0) {
    return deserializeRow(statement);
    
  } else {
    return nullptr;
  }
}

#endif /* SQLiteSensorValueSerializer_hpp */
