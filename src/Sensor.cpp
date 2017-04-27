//
//  Sensor.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 04/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "SensorStatus.hpp"
#include "Sensor.hpp"

Sensor::Sensor(std::string _id)
: id(_id), _enabled(false) {

}

Sensor::~Sensor() {

}

std::string Sensor::getId() {
  return id;
}

void Sensor::setId(std::string id) {
  this->id = id;
}

const bool& Sensor::enabled() const {
  return _enabled;
}

bool& Sensor::enabled() {
  return _enabled;
}
