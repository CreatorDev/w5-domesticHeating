/*
 * Sensor.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include <memory>
#include <string>
#include <vector>

class SensorStatus;

class Sensor {
public:

  virtual ~Sensor();
  virtual void enable() = 0;
  virtual void disable() = 0;
  virtual void update(SensorStatus&) = 0;

  std::string getId();
  void setId(std::string id);
  const bool& enabled() const;
  bool& enabled();

protected:
  Sensor(std::string _id);

private:
  std::string    id;
  bool          _enabled;
};

typedef std::vector<std::shared_ptr<Sensor>> SensorsVector;


#endif /* _SENSOR_HPP_ */
