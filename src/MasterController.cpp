/*
 * MasterController.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: krzysztof.kocon
 */

#include "awa/ServerObserveOperation.hpp"
#include "awa/ServerWriteOperation.hpp"
#include "MasterControler.hpp"

static const char* OBJECT_RESOURCE_INSTANCE_TIME_SCHEDULE = "/20010/0/3500";
static const char* OBJECT_RESOURCE_INSTANCE_TEMP_ID =   "/20010/0/3501";
static const char* OBJECT_RESOURCE_INSTANCE_MOTION_ID = "/20010/1/3501";
static const char* OBJECT_RESOURCE_INSTANCE_RELAY_ID =  "/20010/2/3501";

static void callback(const AwaChangeSet * changeSet, const char * path, void* context)
{
  std::string* lastValRef = static_cast<std::string*>(context);
  const char* value;
  AwaChangeSet_GetValueAsCStringPointer(changeSet, path, &value);
  lastValRef->clear();
  lastValRef->assign(value);
}

static void timeScheduleCallback(const AwaChangeSet * changeSet, void* context){
  callback(changeSet, OBJECT_RESOURCE_INSTANCE_TIME_SCHEDULE, context);
}

static void tempCallback(const AwaChangeSet * changeSet, void* context) {
  callback(changeSet, OBJECT_RESOURCE_INSTANCE_TEMP_ID, context);
}

static void motionCallback(const AwaChangeSet * changeSet, void* context) {
  callback(changeSet, OBJECT_RESOURCE_INSTANCE_MOTION_ID, context);
}

static void relayCallback(const AwaChangeSet * changeSet, void* context) {
  callback(changeSet, OBJECT_RESOURCE_INSTANCE_RELAY_ID, context);
}

MasterController::MasterController(std::string id,
                                   std::shared_ptr<AwaBridge> awaBridge,
                                   MasterControllerData data,
                                   std::shared_ptr<spdlog::logger> logger) :
                                                                   GenericSensor<MasterControllerData>(id, logger),
                                                                   lastValue(data),
                                                                   awaBridge(awaBridge) {

}

MasterController::~MasterController () {
  disable();
}

void initWithDefault(std::string id, const char* resourcePath, const char* value,
                     const awa::ServerSession& session, std::shared_ptr<spdlog::logger> logger) {
  try {
        awa::ServerWriteOperation operation(id, session);
        operation.addValueAsCString(resourcePath, value);
        operation.perform();
    }
    catch(const awa::AwaException& e) {
        logger->warn("MasterController: init with defaults failed! {}", e.what());
    }
}

void MasterController::initWithDefaults() {

  initWithDefault(getId(),
                  OBJECT_RESOURCE_INSTANCE_TIME_SCHEDULE,
                  lastValue.timeSchedule.c_str(),
                  *awaBridge->getSession(),
                  logger);

  initWithDefault(getId(),
                  OBJECT_RESOURCE_INSTANCE_TEMP_ID,
                    lastValue.temperatureSensorId.c_str(),
                    *awaBridge->getSession(),
                    logger);

  initWithDefault(getId(),
                  OBJECT_RESOURCE_INSTANCE_MOTION_ID,
                      lastValue.motionSensorId.c_str(),
                      *awaBridge->getSession(),
                      logger);

  initWithDefault(getId(),
                  OBJECT_RESOURCE_INSTANCE_RELAY_ID,
                        lastValue.actuator.c_str(),
                        *awaBridge->getSession(),
                        logger);
}

void MasterController::enable() {
  if (enabled())
    return;

  if (awaBridge->getSession() == NULL) {
      logger->warn("Enable failed! Session not established.");
      return;
  }

  initWithDefaults();
  timeScheduleObservator = observe (
      OBJECT_RESOURCE_INSTANCE_TIME_SCHEDULE,
      static_cast<void*> (&this->lastValue.timeSchedule), timeScheduleCallback);

  temperatureSensorIdObservator = observe (
      OBJECT_RESOURCE_INSTANCE_TEMP_ID,
      static_cast<void*> (&this->lastValue.temperatureSensorId), tempCallback);

  motionSensorIdObservator = observe (
      OBJECT_RESOURCE_INSTANCE_MOTION_ID,
      static_cast<void*> (&this->lastValue.motionSensorId), motionCallback);

  relaySensorIdObservator = observe (
      OBJECT_RESOURCE_INSTANCE_RELAY_ID,
      static_cast<void*> (&this->lastValue.actuator), relayCallback);

  enabled() = true;

}

std::shared_ptr<awa::ServerObservation> MasterController::observe(const char* resource, void* data, observationCallback cb) {
  const char* sensorName = typeid(this).name();
  logger->info("Subscribing sensor {} to {}", sensorName, resource);
  std::shared_ptr<awa::ServerObservation> observation = make_shared<awa::ServerObservation>(getId(),
                                                                                            resource,
                                                                                            cb,
                                                                                            data );
  awa::ServerObserveOperation operation(*awaBridge->getSession());
  operation.addObservation(*observation);
  operation.perform();
  return observation;
}

void MasterController::disable() {

  if (!enabled())
    return;

  releaseObserver(timeScheduleObservator);
  releaseObserver(temperatureSensorIdObservator);
  releaseObserver(motionSensorIdObservator);
  releaseObserver(relaySensorIdObservator);

  enabled() = false;
}

void MasterController::releaseObserver(std::shared_ptr<awa::ServerObservation> observation) {
  try {
      logger->info("Unsubscribing sensor {}", typeid(this).name());
      awa::ServerObserveOperation cancelObserveOperation(*awaBridge->getSession());
      cancelObserveOperation.addCancelObservation(*observation);
      cancelObserveOperation.perform();
  }
  catch(const std::exception& e) {
      logger->warn("Disable failed! Reason: {}", e.what());
  }
}

void MasterController::update(SensorStatus& status) {
  if (lastValue.timeSchedule.compare(status.masterControllerData.timeSchedule)) {
      status.masterControllerData.timeSchedule = lastValue.timeSchedule;
  }

  if (lastValue.temperatureSensorId.compare(status.masterControllerData.temperatureSensorId)) {
      status.masterControllerData.temperatureSensorId = lastValue.temperatureSensorId;
  }

  if (lastValue.motionSensorId.compare(status.masterControllerData.motionSensorId)) {
      status.masterControllerData.motionSensorId = lastValue.motionSensorId;
  }

  if (lastValue.actuator.compare(status.masterControllerData.actuator)) {
      status.masterControllerData.actuator = lastValue.actuator;
  }
}
