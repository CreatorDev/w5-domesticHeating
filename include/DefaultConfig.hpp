/*
 * DefaultConfig.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _DEFAULTCONFIG_HPP_
#define _DEFAULTCONFIG_HPP_

#include <string>

class DefaultConfig
{
  std::string defaultConfigurationPath;
  std::string timeSchedulePath;
  std::string dbPath;
  std::string temperatureSensorId;
  std::string motionSensorId;
  std::string actuatorSensorId;

  void load();
public:
  DefaultConfig(std::string path);
  virtual ~DefaultConfig();

  const std::string& getTimeSchedulePath() const;
  const std::string& getDatabasePath() const;

  const std::string& getTempSensorId() const;
  void setTempSensorId(std::string id);

  const std::string& getMotionSensorId() const;
  void setMotionSensorId(std::string id);

  const std::string& getActuatorId() const;
  void setActuatorSensorId(std::string id);

  void writeTimeSchedule(std::string timeSchedule);
  std::string readTimeSchedule();

  std::string writeTimeScheduleToTemp(std::string timeSchedule);

  void saveConfiguration();
};

#endif /* _DEFAULTCONFIG_HPP_ */
