/*
 * RelayControlRule.h
 *
 *  Created on: Jan 12, 2017
 *      Author: Zarnowski
 */

#ifndef RelayControlRule_hpp
#define RelayControlRule_hpp

#include "LogicRule.hpp"
#include "AwaBridge.hpp"
#include "DefaultConfig.hpp"

using namespace std;

class RelayControlRule : public LogicRule {
  public:
    RelayControlRule(shared_ptr<AwaBridge>, shared_ptr<DefaultConfig>, shared_ptr<spdlog::logger>);
    virtual ~RelayControlRule();

    virtual void execute(SensorStatus& sensorStatus,
                         SystemStatus& systemStatus,
                         RelaysStatus& relaysStatus) override;

  private:
    shared_ptr<AwaBridge> awaBridge;
    shared_ptr<DefaultConfig> defaultConfig;
};

#endif /* RelayControlRule_hpp */
