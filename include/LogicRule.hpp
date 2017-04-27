//
//  LogicRule.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef LogicRule_hpp
#define LogicRule_hpp

#include <stdio.h>
#include <vector>
#include <memory>
#include "spdlog/logger.h"

using namespace std;

class SensorStatus;
class SystemStatus;
class RelaysStatus;

class LogicRule {

protected:
  std::shared_ptr<spdlog::logger> logger;
public:
  LogicRule(shared_ptr<spdlog::logger>);

  virtual void execute(SensorStatus& sensorStatus,
                       SystemStatus& systemStatus,
                       RelaysStatus& relaysStatus) = 0;
  virtual ~LogicRule() = default;
};

typedef vector<shared_ptr<LogicRule>> RulesVector;

#endif /* LogicRule_hpp */
