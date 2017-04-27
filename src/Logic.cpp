//
//  Logic.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 04/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include <awa/AwaException.hpp>
#include <ConfigurationChangeRule.hpp>
#include <HeaterControlRule.hpp>
#include <letmecreate/core/common.h>
#include <Logic.hpp>
#include <RelayControlRule.hpp>
#include <spdlog/details/logger_impl.h>
#include <spdlog/logger.h>
#include <StandbyRule.hpp>
#include <StoreMeasurementsRule.hpp>
#include <unistd.h>
#include <memory>
#include <stdexcept>
#include <vector>

const unsigned long LOOP_SLEEP_DURATION = 3; //in seconds

Logic::Logic(shared_ptr<DefaultConfig> defaultConfig, std::string address, int port, std::shared_ptr<spdlog::logger> l)
: terminated(false),
  defaultConfig(defaultConfig),
  logger(l),
  schedule( make_shared<Schedule>(defaultConfig->getTimeSchedulePath()) ),
  storage( make_shared<SQLiteStorage>() ),
  awaBridge(make_shared<AwaBridge>(address, port, l)),
  eventWriter(make_shared<EventWriter>(storage, awaBridge, logger)) {

  if (storage->open(defaultConfig->getDatabasePath()) == false) {
      throw invalid_argument("Can't open database");
  }

  buildRules();
}

void Logic::buildRules() {
  //Note: order of rules is important.
  rules.push_back(make_shared<StoreMeasurementsRule>(storage, logger));
  rules.push_back(make_shared<StandbyRule>(eventWriter, logger));
  rules.push_back(make_shared<HeaterControlRule>(eventWriter, schedule, logger));
  rules.push_back(make_shared<RelayControlRule>(awaBridge, defaultConfig, logger));

  rules.push_back(make_shared<ConfigurationChangeRule>(defaultConfig, sensors, schedule, logger));
}

void Logic::handleSensors() {
  for(auto s: sensors) {
      s->update(sensorStatus);
  }
}

void Logic::applyRules() {
  for(auto r: rules) {
      r->execute(sensorStatus, systemStatus, relaysStatus);
  }
}

void Logic::execute() {

  while(terminated == false) {
      try {
          sleep(LOOP_SLEEP_DURATION);

          awaBridge->establishSessionIfNeeded();
          awaBridge->subscribeIfNeeded(sensors);
          awaBridge->process();

          handleSensors();
          applyRules();
      }
      catch (const awa::AwaException& e) {
          logger->warn("Execute: failed! {}", e.what());
          if (awa::AwaException::NotDefined == e.getErrorCode() ||
              awa::AwaException::Timeout == e.getErrorCode() ) { //forwarded on awa-server-write
              awaBridge->unsubscribe(sensors);
              awaBridge->releaseSession();
          }
      }
  }

  awaBridge->unsubscribe(sensors);
}

void Logic::registerSensor(shared_ptr<Sensor> sensor) {
  sensors.push_back(sensor);
}

void Logic::terminate() {
  terminated = true;
}

shared_ptr<AwaBridge> Logic::getAwaBridge() {
  return awaBridge;
}
