/*
 * EventWriter.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: krzysztof.kocon
 */

#include <sstream>
#include "EventWriter.hpp"
#include "awa/ServerWriteOperation.hpp"

static const char* OBJECT_RESOURCE_INSTANCE_EVENT_ID = "/20011/0/5823";

EventWriter::EventWriter (std::shared_ptr<SQLiteStorage> storage,
                          std::shared_ptr<AwaBridge> awaBridge,
                          shared_ptr<spdlog::logger> logger) :
                              storage(storage),
                              awaBridge(awaBridge),
                              logger(logger) {

}

EventWriter::~EventWriter() {

}

void EventWriter::write(SystemEvent event) {
  time_t timestamp = time(nullptr);
  SensorValue<SystemEvent> tmp(-1, -1, event, svtSystemEvent, timestamp);
  auto serializer = storage->getSystemEventSerializer();
  serializer->store(&tmp);

  try {
      std::stringstream ss;
      ss << event << ", " << timestamp;

      awa::ServerWriteOperation operation("MasterControllerClient", *awaBridge->getSession());
      operation.addValueAsCString(OBJECT_RESOURCE_INSTANCE_EVENT_ID, ss.str().c_str());
      operation.perform();
  }
  catch(const awa::AwaException& e) {
      logger->warn("EventWriter: writing SystemEvent! {}", e.what());
  }
}
