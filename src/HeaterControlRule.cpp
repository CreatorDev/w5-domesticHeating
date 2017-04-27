//
//  HeaterControlRule.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include <cstddef>
#include "HeaterControlRule.hpp"
#include "Logic.hpp"
#include "SensorValue.hpp"

HeaterControlRule::HeaterControlRule(shared_ptr<EventWriter> eventWriter,
                                     shared_ptr<Schedule> _schedule,
                                     std::shared_ptr<spdlog::logger> logger)
:  LogicRule(logger), eventWriter(eventWriter), schedule(_schedule), lastHeatingState(false) {
  
}

HeaterControlRule::~HeaterControlRule() {

}
void HeaterControlRule::execute(SensorStatus& sensorStatus, SystemStatus& systemStatus, RelaysStatus& relaysStatus) {
  
  if (systemStatus.standByMode == true) {
    //if we are in standby mode then turn off heating
    logChange( false );
    relaysStatus.heating = false;
  } else {
    relaysStatus.heating = schedule->getDesiredTemperature() > sensorStatus.currentTemperature;
    logChange( relaysStatus.heating );
  }
  logger->debug("[HeaterControlRule] Heating mode: {}", relaysStatus.heating);
}

void HeaterControlRule::logChange(bool const doHeating) {
  
  if (lastHeatingState != doHeating) {
    lastHeatingState = doHeating;
    
    eventWriter->write(doHeating ? seRequestHeating : seStopHeating);
  }
}
