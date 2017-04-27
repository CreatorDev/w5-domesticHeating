/*
 * SensorStatus.hpp
 *
 *  Created on: Feb 25, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _SENSOR_STATUS_HPP
#define _SENSOR_STATUS_HPP

#include "SensorValue.hpp"

struct MasterControllerData {
  MasterControllerData(SystemEvent se = seUnknown,
                       std::string timeSchedule = "",
                       std::string temperatureSensorId = "",
                       std::string motionSensorId = "",
                       std::string actuator = "");
  SystemEvent systemEvent;
  std::string timeSchedule;
  std::string temperatureSensorId;
  std::string motionSensorId;
  std::string actuator;
};

class SensorStatus {
  public:
    bool    movementDetected;
    double  currentTemperature;
    MasterControllerData masterControllerData;

    SensorStatus();
};

class RelaysStatus {
  public:
    bool    heating;
    bool    fan;

    RelaysStatus();
};

class SystemStatus {
  public:
    bool    standByMode;

  SystemStatus();
};

#endif
