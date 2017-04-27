/*
 * AwaBridge.cpp
 *
 *  Created on: Jan 12, 2017
 *      Author: Zarnowski
 */

#include "AwaBridge.hpp"
#include "awa/AwaException.hpp"

AwaBridge::AwaBridge(std::string address, int port, std::shared_ptr<spdlog::logger> l) :
  address(address), port(port), logger(l) {

  logger->debug("[AwaBridge] Creating bridge with IPC configuration: [address: {}, port: {}]", getAddress(), getPort());
}

AwaBridge::~AwaBridge() {
}

void AwaBridge::establishSessionIfNeeded() {
  if (!session) {
      logger->info("Establishing session...");
      allocAndConnect();
  }
}

void AwaBridge::releaseSession() {
  if (session) {
      logger->info("Releasing session...");
      session = NULL;
  }
}

void AwaBridge::subscribeIfNeeded(const std::vector<std::shared_ptr<Sensor>>& sensors) {
  for(auto s: sensors) {
      try {
          s->enable();
      }
      catch (const awa::AwaException& e) {

      }
  }
}

void AwaBridge::unsubscribe(const std::vector<std::shared_ptr<Sensor>>& sensors) {
  for(auto s: sensors) {
      try {
          s->disable();
      }
      catch (const awa::AwaException& e) {

      }
  }
}

void AwaBridge::process() {
  try {

      if (session) {
          session->process();
          session->dispatchCallbacks();
      }
  }
  catch (const AwaError& err)
  {
      logger->warn("Handling session failed! Error: {}", err);
  }
}

std::shared_ptr<awa::ServerSession> AwaBridge::getSession() const {
  return session;
}

const std::string& AwaBridge::getAddress() const {
  return address;
}

const int& AwaBridge::getPort() const {
  return port;
}

void AwaBridge::allocAndConnect()
{
  try {
      session = std::make_shared<awa::ServerSession>();
      session->setIPCAsUDP(getAddress(), getPort());
      session->connect();
  }
  catch(std::bad_alloc& e) {
      logger->warn("Failed to allocate ServerSession!");
  }
  catch (const awa::AwaException& awaError) {
      logger->warn("Awa error occur: {}", awaError.what() );
      session = nullptr;
      throw;
  }
}
