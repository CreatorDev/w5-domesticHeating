/*
 * MasterControler.hpp
 *
 *  Created on: Feb 25, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _MASTERCONTROLER_HPP_
#define _MASTERCONTROLER_HPP_

#include <memory>
#include <tuple>
#include "SensorValue.hpp"
#include "SensorStatus.hpp"
#include "GenericSensor.hpp"
#include "spdlog/logger.h"
#include "AwaBridge.hpp"
#include "awa/ServerObservation.hpp"

typedef void (*observationCallback)(const AwaChangeSet * changeSet, void* context);

class MasterController : public GenericSensor<MasterControllerData>
{
  std::shared_ptr<AwaBridge>           awaBridge;
  MasterControllerData                 lastValue;

  std::shared_ptr<awa::ServerObservation> timeScheduleObservator;
  std::shared_ptr<awa::ServerObservation> temperatureSensorIdObservator;
  std::shared_ptr<awa::ServerObservation> motionSensorIdObservator;
  std::shared_ptr<awa::ServerObservation> relaySensorIdObservator;

  void initWithDefaults();
  std::shared_ptr<awa::ServerObservation> observe(const char* resource, void* data, observationCallback cb);
  void releaseObserver(std::shared_ptr<awa::ServerObservation>);
public:
  MasterController(std::string id, std::shared_ptr<AwaBridge> awaBridge, MasterControllerData data, std::shared_ptr<spdlog::logger> logger);
  virtual ~MasterController();

  virtual void enable() override;
  virtual void disable() override;
  virtual void update(SensorStatus&) override;

};

#endif /* _MASTERCONTROLER_HPP_ */
