//
//  SensorValue.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 03/01/17.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SensorValue_hpp
#define SensorValue_hpp

#include <stdio.h>
#include <time.h>
#include <vector>
#include <memory>

using namespace std;

enum SensorValueType {
  svtUndefined,
  svtTemperature,
  svtMotion,
  svtSystemEvent
};

enum MovementStates {
  msNoMovement,
  msMovementDetected
};

enum SystemEvent {
  seUnknown,
  seEnteringStandBy,
  seLeavingStandBy,
  seRequestHeating,
  seStopHeating,
};

template <typename T>
class SensorValue {
  public:
    SensorValue(long id, long sensId, T value, SensorValueType valType, time_t timestamp);
  
    long getId();
    void setId(long id);
  
    T getValue();
    time_t getTimestamp();

    void setValue(T val);
    void setTimestamp(time_t time);
  
    long getPhysicalSensorId();
    void setPhysicalSensorId(long sensorId);
  
    SensorValueType getValueType();
    void setValueType(SensorValueType value);
  private:
    long    id;
    long    physicalSensorId; 
    T  value;
    time_t  timestamp;
    SensorValueType valueType;
};

typedef SensorValue<double> TemperatureSensorValue;
typedef SensorValue<MovementStates> MovementSensorValue;
typedef SensorValue<SystemEvent> SystemEventValue;

template <typename T>
SensorValue<T>::SensorValue(long _id, long sensId, T _value, SensorValueType valType, time_t _timestamp)
: id(_id), physicalSensorId(sensId), value(_value), timestamp(_timestamp), valueType(valType) {
  
}

template <typename T>
long SensorValue<T>::getId() {
  return id;
}

template <typename T>
void SensorValue<T>::setId(long id) {
  this->id = id;
}

template <typename T>
T SensorValue<T>::getValue() {
  return value;
}

template <typename T>
time_t SensorValue<T>::getTimestamp() {
  return timestamp;
}

template <typename T>
void SensorValue<T>::setValue(T val) {
  value = val;
}

template <typename T>
void SensorValue<T>::setTimestamp(time_t time) {
  timestamp = time;
}

template <typename T>
long SensorValue<T>::getPhysicalSensorId() {
  return physicalSensorId;
}

template <typename T>
void SensorValue<T>::setPhysicalSensorId(long sensorId) {
  this->physicalSensorId = sensorId;
}

template <typename T>
SensorValueType SensorValue<T>::getValueType() {
  return valueType;
}

template <typename T>
void SensorValue<T>::setValueType(SensorValueType value) {
  this->valueType = value;
}

#endif /* SensorValue_hpp */
