//
//  StoreMeasurementsRule.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include <cmath>
#include "StoreMeasurementsRule.hpp"
#include "Logic.hpp"
#include "SensorValue.hpp"
#include "TemperatureSensor.hpp"
#include "MotionSensor.hpp"
#include "SensorTypes.hpp"

const time_t NO_MORE_MOVE_DELAY = 60;       //in seconds
const double TEMPERATURE_DIFF_DELTA = 0.5;

StoreMeasurementsRule::StoreMeasurementsRule(shared_ptr<SQLiteStorage> _storage, std::shared_ptr<spdlog::logger> l)
: LogicRule(l),
  storage(_storage),
  lastMovement(false),
  lastTemperature(0),
  movementDetectedTime(0),
  noMoreMoveStored(false) {
  
}

StoreMeasurementsRule::~StoreMeasurementsRule() {

}

void StoreMeasurementsRule::storeTemperature(SensorStatus& sensorStatus) {
    //Store temperature if delta is exceeded
    if ( std::abs(lastTemperature - sensorStatus.currentTemperature) < TEMPERATURE_DIFF_DELTA) {
        return;
    }
    //store new temperature
    SensorValue<double> tmp(-1, TEMPERATURE_SENSOR_ID, sensorStatus.currentTemperature, svtTemperature, time(nullptr));
    auto serializer = storage->getTemperatureSerializer();
    serializer->store(&tmp);
}

void StoreMeasurementsRule::storeMovement(SensorStatus& sensorStatus) {
    //store movement if needed
    if (lastMovement == false && sensorStatus.movementDetected == true && noMoreMoveStored == true) {
        //detected movement, store into database
        SensorValue<MovementStates> tmp(-1, MOTION_SENSOR_ID, msMovementDetected, svtMotion, time(nullptr));
        auto serializer = storage->getMovementSerializer();
        serializer->store(&tmp);

        noMoreMoveStored = false;
    }

    if (sensorStatus.movementDetected == true) {
        movementDetectedTime = time(nullptr);
    }

    time_t delta = time(nullptr) - movementDetectedTime;
    if (sensorStatus.movementDetected == false && delta > NO_MORE_MOVE_DELAY && noMoreMoveStored == false) {
        //no more movement in wait 60s after last movement detected, store it into database
        noMoreMoveStored = true;

        SensorValue<MovementStates> tmp(-1, MOTION_SENSOR_ID, msNoMovement, svtMotion, time(nullptr));
        auto serializer = storage->getMovementSerializer();
        serializer->store(&tmp);
    }
}

void StoreMeasurementsRule::execute(SensorStatus& sensorStatus, SystemStatus& systemStatus, RelaysStatus& relaysStatus) {
    storeTemperature(sensorStatus);
    storeMovement(sensorStatus);

    lastMovement = sensorStatus.movementDetected;
    lastTemperature = sensorStatus.currentTemperature;
}
