/*
 * ServerObservation.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: krzysztof.kocon
 */

#include <awa/ServerObservation.hpp>

using namespace awa;


ServerObservation::ServerObservation(std::string clientId,
                                     std::string resourcePath,
                                     AwaServerObservationCallback callback,
                                     void* context) {
  observation = AwaServerObservation_New(clientId.c_str(), resourcePath.c_str(), callback, context);
  if (NULL == observation) {
      throw std::bad_alloc();
  }

}

ServerObservation::~ServerObservation() {
  if (observation) {
      AwaServerObservation_Free(&observation);
  }
}


ServerObservation::operator AwaServerObservation*() const {
  return observation;
}
