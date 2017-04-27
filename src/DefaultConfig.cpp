/*
 * DefaultConfig.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: krzysztof.kocon
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "DefaultConfig.hpp"
#include "json/json.hpp"

using json = nlohmann::json;

static const char* TIME_SCHEDULE_PATH="timeSchedulePath";
static const char* DB_PATH="dbPath";
static const char* TEMPERATURE_SENSOR_ID="temperatureSensorId";
static const char* MOTION_SENSOR_ID="motionSensorId";
static const char* ACTUATOR_SENSOR_ID="actuator";

DefaultConfig::DefaultConfig(std::string path) : defaultConfigurationPath(path) {

  load();
}

DefaultConfig::~DefaultConfig () {

}

void DefaultConfig::load() {
  std::ifstream fis(defaultConfigurationPath);
  if (fis.good() == false) {
      throw std::invalid_argument("Can't open default configuration file:" + defaultConfigurationPath);
  }

  std::stringstream buffer;
  buffer << fis.rdbuf();

  fis.close();
  json config = json::parse(buffer.str());

  //TODO: add sanity checks

  this->timeSchedulePath = config[TIME_SCHEDULE_PATH].get<std::string>();
  this->dbPath = config[DB_PATH].get<std::string>();
  this->temperatureSensorId = config[TEMPERATURE_SENSOR_ID].get<std::string>();
  this->motionSensorId = config[MOTION_SENSOR_ID].get<std::string>();
  this->actuatorSensorId = config[ACTUATOR_SENSOR_ID].get<std::string>();

}

const std::string& DefaultConfig::getTimeSchedulePath() const {
  return this->timeSchedulePath;
}

const std::string& DefaultConfig::getDatabasePath() const {
  return dbPath;
}

const std::string& DefaultConfig::getTempSensorId() const {
  return this->temperatureSensorId;
}

const std::string& DefaultConfig::getMotionSensorId() const {
  return this->motionSensorId;
}

const std::string& DefaultConfig::getActuatorId() const {
  return this->actuatorSensorId;
}

void DefaultConfig::setTempSensorId(std::string id) {
  this->temperatureSensorId = id;
}

void DefaultConfig::setMotionSensorId(std::string id) {
  this->motionSensorId = id;
}

void DefaultConfig::setActuatorSensorId(std::string id) {
  this->actuatorSensorId = id;
}

std::string DefaultConfig::readTimeSchedule() {
  std::ifstream file(getTimeSchedulePath());
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void DefaultConfig::writeTimeSchedule(std::string timeSchedule) {
  std::ofstream out(getTimeSchedulePath());
  if (!out) {
      return;
  }

  out << timeSchedule;
  out.close();

}

std::string DefaultConfig::writeTimeScheduleToTemp(std::string timeSchedule) {
  std::string path = getTimeSchedulePath()+".tmp";
  std::ofstream out(path);
  if (!out) {
      return nullptr;
  }

  out << timeSchedule;
  out.close();
  return path;
}

void DefaultConfig::saveConfiguration() {
  json jsonData = {
      {TIME_SCHEDULE_PATH , getTimeSchedulePath()},
      {DB_PATH, getDatabasePath()},
      {TEMPERATURE_SENSOR_ID, getTempSensorId()},
      {MOTION_SENSOR_ID, getMotionSensorId()},
      {ACTUATOR_SENSOR_ID, getActuatorId()}
    };

  std::ofstream outputFileStream(defaultConfigurationPath);
  outputFileStream << jsonData.dump();
  bool result = !outputFileStream.bad();
  outputFileStream.close();
  if (result == false) {
      printf("Failed to write %s", defaultConfigurationPath);
  }
}
