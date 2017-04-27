/*
 * EventWriter.hpp
 *
 *  Created on: Mar 1, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _EVENTWRITER_HPP_
#define _EVENTWRITER_HPP_

#include <memory>
#include "SQLiteStorage.hpp"
#include "AwaBridge.hpp"
#include "SensorValue.hpp"

class EventWriter
{
  std::shared_ptr<SQLiteStorage> storage;
  std::shared_ptr<AwaBridge> awaBridge;
  shared_ptr<spdlog::logger> logger;
public:
  EventWriter (std::shared_ptr<SQLiteStorage>, std::shared_ptr<AwaBridge>, shared_ptr<spdlog::logger>);
  virtual ~EventWriter ();

  void write(SystemEvent event);
};

#endif /* _EVENTWRITER_HPP_ */
