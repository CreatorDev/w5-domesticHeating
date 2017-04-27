//
//  GenericSensor.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 04/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef GenericSensor_hpp
#define GenericSensor_hpp

#include <stdexcept>
#include <memory>
#include "spdlog/spdlog.h"
#include "Sensor.hpp"

template <typename T>
class GenericSensor : public Sensor {

public:
  T getValue() {
    T d;
    innerGet(d);
    return d;
  }
protected:
  GenericSensor(std::string _id, std::shared_ptr<spdlog::logger> l) : Sensor(_id), logger(l) {}
  virtual ~GenericSensor() {};

  virtual void innerGet(int& value) { throw std::logic_error("Expected override!"); };
  virtual void innerGet(bool& value) { throw std::logic_error("Expected override!");};
  virtual void innerGet(double& value) { throw std::logic_error("Expected override!");};

  std::shared_ptr<spdlog::logger> logger;
};

#endif /* GenericSensor_hpp */
