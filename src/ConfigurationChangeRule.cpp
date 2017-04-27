/*
 * ConfigurationChangeRule.cpp
 *
 *  Created on: Feb 28, 2017
 *      Author: krzysztof.kocon
 */

#include <stdexcept>
#include "ConfigurationChangeRule.hpp"
#include "TemperatureSensor.hpp"
#include "MotionSensor.hpp"

ConfigurationChangeRule::ConfigurationChangeRule(std::shared_ptr<DefaultConfig> defaultConfig,
                                                 const SensorsVector& sensors,
                                                 std::shared_ptr<Schedule> schedule,
                                                 shared_ptr<spdlog::logger> logger) :

                                                 LogicRule(logger),
                                                 defaultConfig(defaultConfig),
                                                 sensors(sensors),
                                                 schedule(schedule),
                                                 lastConfigHash(schedule->getHash()) {

}

ConfigurationChangeRule::~ConfigurationChangeRule () {

}

void ConfigurationChangeRule::execute(SensorStatus& sensorStatus,
                                      SystemStatus& systemStatus,
                                      RelaysStatus& relaysStatus) {

  std::size_t hash =  std::hash<std::string>{}(sensorStatus.masterControllerData.timeSchedule);
  if (hash != lastConfigHash) {

      try {
          //Schedule (er) should verify data!
          std::string tmp = defaultConfig->writeTimeScheduleToTemp(sensorStatus.masterControllerData.timeSchedule);
          Schedule s(tmp);

          //handle real data
          logger->debug("[ConfigurationChangeRule] Time schedule changed. Updating...");
          defaultConfig->writeTimeSchedule(sensorStatus.masterControllerData.timeSchedule);
          schedule->loadFromConfig(defaultConfig->getTimeSchedulePath());
          lastConfigHash = schedule->getHash();
      }
      catch (std::logic_error& e) {
          lastConfigHash = hash;
          logger->warn("[ConfigurationChangeRule] Time schedule verification failed. {}", e.what());
      }
  }

  bool needWrite = false;

  if (defaultConfig->getTempSensorId().compare(sensorStatus.masterControllerData.temperatureSensorId) != 0 &&
      sensorStatus.masterControllerData.temperatureSensorId.empty() == false) {
      std::shared_ptr<Sensor> sensor = getSensor<TemperatureSensor>(sensors);
      if (sensor) {
          logger->info("[ConfigurationChangeRule] Temperature sensor ID changed from {} to {}. Updating...",
                        sensor->getId(), sensorStatus.masterControllerData.temperatureSensorId);

          sensor->disable();
          sensor->setId(sensorStatus.masterControllerData.temperatureSensorId);
          defaultConfig->setTempSensorId(sensorStatus.masterControllerData.temperatureSensorId);
          needWrite = true;
      }
  }

  if (defaultConfig->getMotionSensorId().compare(sensorStatus.masterControllerData.motionSensorId) != 0 &&
      sensorStatus.masterControllerData.motionSensorId.empty() == false) {
      std::shared_ptr<Sensor> sensor = getSensor<MotionSensor>(sensors);
      if (sensor) {
          logger->info("[ConfigurationChangeRule] Motion sensor ID changed from {} to {}. Updating...",
                                  sensor->getId(), sensorStatus.masterControllerData.motionSensorId);

          sensor->disable();
          sensor->setId(sensorStatus.masterControllerData.motionSensorId);
          defaultConfig->setMotionSensorId(sensorStatus.masterControllerData.motionSensorId);
          needWrite = true;
      }
  }

  if (defaultConfig->getActuatorId().compare(sensorStatus.masterControllerData.actuator) != 0 &&
      sensorStatus.masterControllerData.actuator.empty() == false) {

      logger->info("[ConfigurationChangeRule] Actuator sensor ID changed to {}. Updating...",
                    sensorStatus.masterControllerData.actuator);

      defaultConfig->setActuatorSensorId(sensorStatus.masterControllerData.actuator);
      needWrite = true;
  }

  if (needWrite) {
      defaultConfig->saveConfiguration();
  }

}

