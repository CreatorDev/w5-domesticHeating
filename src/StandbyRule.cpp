//
//  StandbyRule.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "StandbyRule.hpp"
#include "Logic.hpp"
#include "SQLiteStorage.hpp"

static const time_t STANDBY_TRIGGER_DELAY = 2 * 60 * 60;  //in seconds

StandbyRule::StandbyRule(shared_ptr<EventWriter> eventWriter, shared_ptr<spdlog::logger> l) :
    LogicRule(l), eventWriter(eventWriter), lastMovementTime(0) {
  
}
void StandbyRule::handleStandbyMode(SensorStatus& sensorStatus, SystemStatus& systemStatus) {
    if (sensorStatus.movementDetected == false) {
        return;
    }
    systemStatus.standByMode = false;
    lastMovementTime = time(nullptr);

    eventWriter->write(seLeavingStandBy);
}

void StandbyRule::handleActiveMode(SensorStatus& sensorStatus, SystemStatus& systemStatus) {
    //detect end of movement
    if (sensorStatus.movementDetected) {
        lastMovementTime = time(nullptr);

    }

    //detect if movement was not seen for some defined time
    time_t delta = time(nullptr) - lastMovementTime;
    if (sensorStatus.movementDetected == false && delta > STANDBY_TRIGGER_DELAY) {
        systemStatus.standByMode = true;

        eventWriter->write(seEnteringStandBy);
    }
}

void StandbyRule::execute(SensorStatus& sensorStatus, SystemStatus& systemStatus, RelaysStatus& relaysStatus) {
    if (systemStatus.standByMode) {
        handleStandbyMode(sensorStatus, systemStatus);

    } else {
        handleActiveMode(sensorStatus, systemStatus);
    }
    logger->debug("[StandbyRule] Movement detected: {}", sensorStatus.movementDetected);
}
