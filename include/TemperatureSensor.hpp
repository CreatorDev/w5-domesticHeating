//
//  TemperatureSensor.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 04/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef TemperatureSensor_hpp
#define TemperatureSensor_hpp

#include <stdio.h>
#include <memory>
#include "GenericSensor.hpp"
#include "AwaBridge.hpp"

#include "awa/ServerObservation.hpp"

using namespace std;

class TemperatureSensor : public GenericSensor<double> {
  public:
    TemperatureSensor(string id, shared_ptr<AwaBridge> awaBridge, std::shared_ptr<spdlog::logger> logger);
    virtual ~TemperatureSensor() override;

    virtual void enable() override;
    virtual void disable() override;
    virtual void update(SensorStatus&) override;
  protected:
    double                          lastValue;
    shared_ptr<AwaBridge>           awaBridge;
    shared_ptr<awa::ServerObservation> observation;

    virtual void innerGet(double& value) override;
};
#endif /* TemperatureSensor_hpp */
