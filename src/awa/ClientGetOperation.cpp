/*
 * ClientGetOperation.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: krzysztof.kocon
 */

#include "awa/Common.hpp"
#include "awa/Consts.hpp"
#include "awa/ClientSession.hpp"
#include "awa/ClientGetOperation.hpp"

using namespace awa;


ClientGetOperation::ClientGetOperation(const ClientSession& session) throw (std::bad_alloc) {

  operation = AwaClientGetOperation_New(session);
  if (operation == NULL) {
      throw std::bad_alloc();
  }
}

ClientGetOperation::~ClientGetOperation () {
  if (operation) {
       try {
           AWA_CHECK(AwaClientGetOperation_Free(&operation));
       }
       catch(const AwaException& e) {
         //skip
       }
   }
}

void ClientGetOperation::addPath(const char* resourcePath) throw (AwaException) {
  AWA_CHECK(AwaClientGetOperation_AddPath(operation, resourcePath));
}

void ClientGetOperation::perform() throw (AwaException) {
  AWA_CHECK(AwaClientGetOperation_Perform(operation, AWA_OPERATION_PERFORM_TIMEOUT));
}

ClientGetOperation::operator AwaClientGetOperation*() const {
  return operation;
}
