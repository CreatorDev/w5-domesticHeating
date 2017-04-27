/*
 * ServerObserveOperation.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: krzysztof.kocon
 */

#include <awa/common.h>

#include "awa/Consts.hpp"
#include "awa/Common.hpp"
#include "awa/ServerSession.hpp"
#include "awa/ServerObservation.hpp"
#include "awa/ServerObserveOperation.hpp"

using namespace awa;

ServerObserveOperation::ServerObserveOperation(const ServerSession& session) throw (std::bad_alloc) {
  operation = AwaServerObserveOperation_New(session);
  if (NULL == operation) {
      throw std::bad_alloc();
  }
}

ServerObserveOperation::~ServerObserveOperation() {
  if (operation) {
      AwaServerObserveOperation_Free(&operation);
  }
}

void ServerObserveOperation::addObservation(const ServerObservation& observation) throw (AwaException){
  AWA_CHECK(AwaServerObserveOperation_AddObservation(operation, observation));

}

void ServerObserveOperation::addCancelObservation(const ServerObservation& observation) throw (AwaException) {
  AWA_CHECK(AwaServerObserveOperation_AddCancelObservation(operation, observation));
}

void ServerObserveOperation::perform() throw (AwaException) {
  AWA_CHECK(AwaServerObserveOperation_Perform(operation, AWA_OPERATION_PERFORM_TIMEOUT));
}

