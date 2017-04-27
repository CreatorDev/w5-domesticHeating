//
//  main.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 09/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <stdexcept>
#include "Application.hpp"
#include "tclap/CmdLine.h"

static const char* DEFAULT_CONFIG_FILE = "default.cfg";

/** Local setup by default. */
static std::string DEFAULT_ADDRESS = "127.0.0.1";
static const int DEFAULT_PORT = 54321;

int main(int argc, const char * argv[]) {

  try {

      TCLAP::CmdLine cmd("Domestic heating controller", ' ', "0.1");
      TCLAP::ValueArg<string> cfg("c", "config", "Path to a default configuration file.",
                                         true, DEFAULT_CONFIG_FILE, "path");
      cmd.add( cfg );

      TCLAP::ValueArg<string> address("a", "address", "The IP address to use to connect with the awa server.",
                                               false, DEFAULT_ADDRESS, "address");
      cmd.add(address);

      TCLAP::ValueArg<int> port("p", "port", "The PORT number to use to connect with the awa server.",
                                                     false, DEFAULT_PORT, "port");
      cmd.add(port);
      //TODO: add logger configuration

      cmd.parse( argc, argv );

      Application app(cfg.getValue(), address.getValue(), port.getValue());
      app.run();
  }
  catch(const TCLAP::ArgException& e) {
       std::cout << "ERROR: " << e.error() << " " << e.argId() << std::endl;
  }
  catch(std::exception& e) {
      std::cout << "Fatal error! " << e.what() <<std::endl;
  }
}
