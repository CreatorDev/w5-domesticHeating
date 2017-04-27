/*
 * Application.hpp
 *
 *  Created on: Feb 3, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

#include <iostream>
#include <stdexcept>
#include "DefaultConfig.hpp"
#include "Logic.hpp"
#include "spdlog/spdlog.h"

namespace spd = spdlog;

class Config;

class Application
{

  /**
   * Sig action handler.
   */
  class SignalHandler {
      static Application* app;
  public:
      explicit SignalHandler(Application* application);
      ~SignalHandler();

      static void handler(int signo);
  };

  static void installSignalActions();

  std::shared_ptr<SignalHandler>  sigHandler;
  std::shared_ptr<spdlog::logger> logger;
  std::shared_ptr<DefaultConfig>  defaultConfig;
  std::shared_ptr<Logic>          logic;

  void handle(int signo);
  void handleException(std::exception_ptr);

  void setupSensors();

public:
  explicit Application(const std::string configurationPath, const string address, int port);
  virtual ~Application ();
  int run();


};

#endif /* _APPLICATION_HPP_ */
