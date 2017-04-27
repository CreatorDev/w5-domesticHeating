//
//  TemperatureSensor.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 04/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "SensorStatus.hpp"
#include "spdlog/logger.h"
#include "awa/ServerSession.hpp"
#include "awa/ServerObserveOperation.hpp"
#include "TemperatureSensor.hpp"
#include "SensorTypes.hpp"

static const char* OBJECT_RESOURCE_INSTANCE = "/3303/0/5700";

static void changeCallback(const AwaChangeSet * changeSet, void* context)
{
  double* lastValRef = static_cast<double*>(context);
  double lastValue = *lastValRef;
  const AwaFloat* value;
  AwaChangeSet_GetValueAsFloatPointer(changeSet, OBJECT_RESOURCE_INSTANCE, &value);
  *lastValRef = static_cast<double>(*value);
  spdlog::get("console")->info("[TemperatureSensor] temperature changed : {:3.3f} -> {:3.3f}", lastValue, *lastValRef);
}

TemperatureSensor::TemperatureSensor(std::string id,
                                     shared_ptr<AwaBridge> _awaBridge,
                                     std::shared_ptr<spdlog::logger> logger)
: GenericSensor<double>(id, logger),
  lastValue(0),
  awaBridge(_awaBridge),
  observation(nullptr) {

}

TemperatureSensor::~TemperatureSensor() {
  if (observation != nullptr) {
      disable();
  }
}

void TemperatureSensor::innerGet(double& value) {
  value = lastValue;
}

void TemperatureSensor::enable() {
  try {
      if (enabled())
        return;

      if (awaBridge->getSession() == NULL) {
          logger->warn("Enable failed! Session not established.");
          return;
      }

      const char* sensorName = typeid(this).name();

      logger->info("Subscribing sensor {} to {}", sensorName, OBJECT_RESOURCE_INSTANCE);
      observation = make_shared<awa::ServerObservation>(getId(),
                                                        OBJECT_RESOURCE_INSTANCE,
                                                        changeCallback,
                                                        static_cast<void*>(&this->lastValue) );
      awa::ServerObserveOperation operation(*awaBridge->getSession());
      operation.addObservation(*observation);
      operation.perform();

      enabled() = true;
  }
  catch(const awa::AwaException& awa_e) {
      observation = NULL;
      logger->warn("Enable failed! {}", awa_e.what());
  }
  catch(const std::exception& e) {
      logger->warn("Enable failed! {}", e.what());
  }
}

void TemperatureSensor::disable() {
  try {
      if (!enabled())
        return;

      logger->info("Unsubscribing sensor {}", typeid(this).name());
      awa::ServerObserveOperation cancelObserveOperation(*awaBridge->getSession());
      cancelObserveOperation.addCancelObservation(*observation);
      cancelObserveOperation.perform();

      enabled() = false;
      observation = NULL;
  }
  catch(const std::exception& e) {
      logger->warn("Disable failed! Reason: {}", e.what());
  }
}

void TemperatureSensor::update(SensorStatus& sensorStatus) {
  sensorStatus.currentTemperature = getValue();
  logger->debug("[TemperatureSensor] Current temperature: {:3.3f}", sensorStatus.currentTemperature);
}
