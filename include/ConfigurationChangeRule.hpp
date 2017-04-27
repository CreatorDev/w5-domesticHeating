/*
 * ConfigurationChangeRule.hpp
 *
 *  Created on: Feb 28, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _CONFIGURATIONCHANGERULE_HPP_
#define _CONFIGURATIONCHANGERULE_HPP_

#include <memory>
#include "Sensor.hpp"
#include "SensorStatus.hpp"
#include "DefaultConfig.hpp"
#include "LogicRule.hpp"
#include "Schedule.hpp"

class ConfigurationChangeRule :  public LogicRule {
  std::shared_ptr<DefaultConfig> defaultConfig;
  const SensorsVector& sensors;
  std::shared_ptr<Schedule> schedule;
  std::size_t lastConfigHash;
public:
  ConfigurationChangeRule(std::shared_ptr<DefaultConfig>,
                          const SensorsVector&,
                          std::shared_ptr<Schedule>,
                          shared_ptr<spdlog::logger>);
  virtual ~ConfigurationChangeRule();

  void execute(SensorStatus& sensorStatus,
                           SystemStatus& systemStatus,
                           RelaysStatus& relaysStatus) override;
};


template <typename T>
std::shared_ptr<Sensor> getSensor(const SensorsVector& sensors) {

  for (auto s: sensors) {

      if (dynamic_cast<T*>(s.get())) {
          return s;
      }
  }
  return nullptr;
}


#endif /* _CONFIGURATIONCHANGERULE_HPP_ */
