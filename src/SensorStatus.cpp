#include "SensorStatus.hpp"

MasterControllerData::MasterControllerData(SystemEvent se,
                                           std::string timeSchedule,
                                           std::string temperatureSensorId,
                                           std::string motionSensorId,
                                           std::string actuator) :
                                        systemEvent(se),
                                        timeSchedule(timeSchedule),
                                        temperatureSensorId(temperatureSensorId),
                                        motionSensorId(motionSensorId),
                                        actuator(actuator) {

}

SensorStatus::SensorStatus()
: movementDetected(false), currentTemperature(0), masterControllerData() {

}

RelaysStatus::RelaysStatus()
: heating(false), fan(false) {

}

SystemStatus::SystemStatus()
: standByMode(false) {

}
