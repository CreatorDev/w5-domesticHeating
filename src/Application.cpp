/*
 * Application.cpp
 *
 *  Created on: Feb 3, 2017
 *      Author: krzysztof.kocon
 */

#include "Application.hpp"
#include "MotionSensor.hpp"
#include "TemperatureSensor.hpp"
#include "MasterControler.hpp"
#include "SensorValue.hpp"
#include <signal.h>


static void setupLogger(std::shared_ptr<spdlog::logger> logger);
static void setupSensors(Logic& logic, std::shared_ptr<spdlog::logger> logger);

Application* Application::SignalHandler::app = nullptr;

Application::SignalHandler::SignalHandler(Application* application)
{
  app = application;
}

Application::SignalHandler::~SignalHandler() {
  app = nullptr;
}

void Application::SignalHandler::handler(int signo)
{
  if (app) {
      app->handle(signo);
  }
}

Application::Application(const std::string configurationPath, std::string address, int port) :
            sigHandler(new SignalHandler(this)),
            logger(spd::stdout_color_mt("console")),
            defaultConfig(make_shared<DefaultConfig>(configurationPath)),
            logic(make_shared<Logic>(this->defaultConfig, address, port, logger)) {

  setupLogger(logger);
  setupSensors();
  installSignalActions();
}

Application::~Application ()
{
  logic->terminate();
  spdlog::drop_all();
}

void Application::installSignalActions()
{
  struct sigaction action;
  action.sa_handler = Application::SignalHandler::handler;
  action.sa_flags = 0;
  sigemptyset (&action.sa_mask);
  sigaction (SIGINT, &action, NULL);
  sigaction (SIGTERM, &action, NULL);

}

int Application::run()
{
  try {
      logger->info("Starting application main loop...");
      logic->execute();
  }
  catch (...) {
      auto eptr = std::current_exception();
      handleException(eptr);
  }
  logger->info("Finishing the application...");
  return 0;
}

void Application::handle(int signo) {

  fflush(stdout);
  fflush(stderr);

  //TODO: add proper handling of signo

  logic->terminate();
  logger->warn("---> Terminating application");
}

void Application::handleException(std::exception_ptr eptr) {
  try {
      if (eptr) {
          std::rethrow_exception(eptr);
      }
  }
  catch (const std::exception& e) {
      logger->error("Exception handler triggered with: {}", e.what());
  }
}

void Application::setupSensors() {


  auto motionSensor = std::make_shared<MotionSensor>(defaultConfig->getMotionSensorId(), logic->getAwaBridge(), logger);
  auto temperatureSensor = std::make_shared<TemperatureSensor>(defaultConfig->getTempSensorId(),logic->getAwaBridge(), logger);

  MasterControllerData data(SystemEvent::seUnknown,
                            defaultConfig->readTimeSchedule(),
                            defaultConfig->getTempSensorId(),
                            defaultConfig->getMotionSensorId(),
                            defaultConfig->getActuatorId());
  auto masterControllerSensor = std::make_shared<MasterController>("MasterControllerClient",
                                                                   logic->getAwaBridge(),
                                                                   data,
                                                                   logger);

  logic->registerSensor(motionSensor);
  logic->registerSensor(temperatureSensor);
  logic->registerSensor(masterControllerSensor);
}

static void setupLogger(std::shared_ptr<spdlog::logger> logger) {
  logger->set_level(spd::level::debug);
}
