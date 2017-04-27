//
//  Logic.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 04/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef Logic_hpp
#define Logic_hpp

#include <stdio.h>
#include <string>
#include "spdlog/spdlog.h"
#include "SensorStatus.hpp"
#include "DefaultConfig.hpp"
#include "Schedule.hpp"
#include "SQLiteStorage.hpp"
#include "Sensor.hpp"
#include "GenericSensor.hpp"
#include "LogicRule.hpp"
#include "EventWriter.hpp"
#include "AwaBridge.hpp"


class Logic {

  public:
    Logic(shared_ptr<DefaultConfig>, std::string address, int port, shared_ptr<spdlog::logger> logger);
    void registerSensor(shared_ptr<Sensor> sensor);
    void execute();
    void terminate();

    shared_ptr<AwaBridge> getAwaBridge();
  private:
    shared_ptr<DefaultConfig>   defaultConfig;
    shared_ptr<spdlog::logger>  logger;
    bool                        terminated;

    shared_ptr<Schedule>        schedule;
    shared_ptr<SQLiteStorage>   storage;
    SensorsVector               sensors;
    RulesVector                 rules;
    shared_ptr<AwaBridge>       awaBridge;
    shared_ptr<EventWriter>     eventWriter;
  
    SensorStatus  sensorStatus;
    SystemStatus  systemStatus;
    RelaysStatus  relaysStatus;

    void handleSensors();
    void buildRules();
    void applyRules();
  
    template <typename T> T* getSensorAs(shared_ptr<Sensor> sensor) {
      return static_cast<T*>(sensor.get());
    }
};
#endif /* Logic_hpp */
