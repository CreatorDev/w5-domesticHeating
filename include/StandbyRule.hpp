//
//  StandbyRule.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef StandbyRule_hpp
#define StandbyRule_hpp

#include <stdio.h>
#include <time.h>
#include <memory>
#include "LogicRule.hpp"
#include "Schedule.hpp"
#include "EventWriter.hpp"

using namespace std;

class StandbyRule : public LogicRule {
  public:
    virtual void execute(SensorStatus& sensorStatus,
                         SystemStatus& systemStatus,
                         RelaysStatus& relaysStatus) override;
    
    StandbyRule(shared_ptr<EventWriter> eventWriter, shared_ptr<spdlog::logger> l);
  private:
    shared_ptr<EventWriter>   eventWriter;
    time_t                    lastMovementTime;

    void handleStandbyMode(SensorStatus& sensorStatus, SystemStatus& systemStatus);
    void handleActiveMode(SensorStatus& sensorStatus, SystemStatus& systemStatus);
};

#endif /* StandbyRule_hpp */
