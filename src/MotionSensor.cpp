//
//  MotionSensor.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 04/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "SensorStatus.hpp"
#include "spdlog/logger.h"
#include "awa/ServerSession.hpp"
#include "awa/ServerObserveOperation.hpp"
#include "MotionSensor.hpp"
#include "SensorTypes.hpp"

static const char* OBJECT_RESOURCE_INSTANCE = "/3302/0/5500";

static void changeCallback(const AwaChangeSet * changeSet, void* context)
{
  bool* lastValRef = static_cast<bool*>(context);
  bool lastValue = *lastValRef;
  const AwaBoolean* value;
  AwaChangeSet_GetValueAsBooleanPointer(changeSet, OBJECT_RESOURCE_INSTANCE, &value);
  *lastValRef = (*value) != 0;
  spdlog::get("console")->info("[MotionSensor] motion changed : {} -> {}", lastValue, *lastValRef);
}

MotionSensor::MotionSensor(std::string id,
                           shared_ptr<AwaBridge> awaBridge,
                           shared_ptr<spdlog::logger> l)
: GenericSensor<bool>(id, l),
  lastValue(false),
  awaBridge(awaBridge),
  observation(nullptr) {
}
MotionSensor::~MotionSensor() {
  if (observation != nullptr) {
      disable();
  }
}

void MotionSensor::innerGet(bool& value) {
  value = lastValue;
}

void MotionSensor::enable() {
  try {
      if (enabled())
        return;

      if (awaBridge->getSession() == NULL) {
          logger->warn("Enable failed! Session not established.");
          return;
      }

      const char* sensorName = typeid(this).name();

      //
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

void MotionSensor::disable() {
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

void MotionSensor::update(SensorStatus& sensorStatus) {

  sensorStatus.movementDetected = getValue();
  logger->debug("[MotionSensor] Movement detected: {}", sensorStatus.movementDetected);
}
