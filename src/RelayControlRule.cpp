/*
 * RelayControlRule.cpp
 *
 *  Created on: Jan 12, 2017
 *      Author: Zarnowski
 */

#include "RelayControlRule.hpp"
#include "Logic.hpp"
#include "awa/AwaException.hpp"
#include "awa/ServerWriteOperation.hpp"

static const char* OBJECT_RESOURCE_INSTANCE = "/3201/0/5550";

RelayControlRule::RelayControlRule(shared_ptr<AwaBridge> awaBridge,
                                   shared_ptr<DefaultConfig> defaultConfig,
                                   shared_ptr<spdlog::logger> logger)
: LogicRule(logger), awaBridge(awaBridge), defaultConfig(defaultConfig) {
}

RelayControlRule::~RelayControlRule() {

}

void RelayControlRule::execute(SensorStatus& sensorStatus, SystemStatus& systemStatus, RelaysStatus& relaysStatus) {

  try {
      awa::ServerWriteOperation operation(defaultConfig->getActuatorId(), *awaBridge->getSession());
      operation.addValueAsBoolean(OBJECT_RESOURCE_INSTANCE, relaysStatus.heating);
      operation.perform();
      logger->debug("[RelayControlRule] Relay switch: {}", relaysStatus.heating);
  }
  catch (awa::AwaException& e) {
      logger->warn("RelayControlRule failed! {}", e.what());
      if (awa::AwaException::Response == e.getErrorCode()) { //operation has error
          //skip
      }
  }

}

