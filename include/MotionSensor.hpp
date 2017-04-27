//
//  MotionSensor.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 04/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef MotionSensor_hpp
#define MotionSensor_hpp

#include <stdio.h>
#include <string>
#include <memory>
#include "GenericSensor.hpp"
#include "AwaBridge.hpp"


#include "awa/ServerObservation.hpp"

using namespace std;

class MotionSensor : public GenericSensor<bool> {
  public:
    MotionSensor(string id, shared_ptr<AwaBridge> awaBridge, shared_ptr<spdlog::logger> l);
    ~MotionSensor();
    
    virtual void enable() override;
    virtual void disable() override;
    virtual void update(SensorStatus&) override;
  protected:
    bool                            lastValue;
    shared_ptr<AwaBridge>           awaBridge;
    shared_ptr<awa::ServerObservation> observation;


    virtual void innerGet(bool& value) override;
};
#endif /* MotionSensor_hpp */
