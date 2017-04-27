//
//  StoreMeasurementsRule.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef StoreMeasurementsRule_hpp
#define StoreMeasurementsRule_hpp

#include <stdio.h>
#include <time.h>
#include "LogicRule.hpp"
#include "SQLiteStorage.hpp"

class StoreMeasurementsRule : public LogicRule {
  public:
    StoreMeasurementsRule(std::shared_ptr<SQLiteStorage>, std::shared_ptr<spdlog::logger>);
    virtual ~StoreMeasurementsRule();

    virtual void execute(SensorStatus& sensorStatus,
                         SystemStatus& systemStatus,
                         RelaysStatus& relaysStatus) override;
  private:
    std::shared_ptr<SQLiteStorage>   storage;
    bool    lastMovement;
    double  lastTemperature;
    time_t  movementDetectedTime;

    //flag which mark situation when end of movement was stored in database
    bool    noMoreMoveStored;

    void storeTemperature(SensorStatus& sensorStatus);
    void storeMovement(SensorStatus& sensorStatus);
};

#endif /* StoreMeasurementsRule_hpp */
