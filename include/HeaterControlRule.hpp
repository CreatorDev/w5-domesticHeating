//
//  HeaterControlRule.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef HeaterControlRule_hpp
#define HeaterControlRule_hpp

#include <stdio.h>
#include <memory>
#include "LogicRule.hpp"
#include "EventWriter.hpp"
#include "Schedule.hpp"
#include "spdlog/logger.h"
#include <cstddef>

using namespace std;

class HeaterControlRule : public LogicRule {
  public:
    HeaterControlRule(shared_ptr<EventWriter>, shared_ptr<Schedule>, std::shared_ptr<spdlog::logger>);
    virtual ~HeaterControlRule();

    virtual void execute(SensorStatus& sensorStatus,
                         SystemStatus& systemStatus,
                         RelaysStatus& relaysStatus) override;
    
  private:
    shared_ptr<EventWriter>     eventWriter;
    shared_ptr<Schedule>        schedule;
    bool                        lastHeatingState;
  
    void  logChange(bool const doHeating);
};
#endif /* HeaterControlRule_hpp */
