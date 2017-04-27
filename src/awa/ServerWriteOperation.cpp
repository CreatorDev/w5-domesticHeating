/*
 * ServerWriteOperation.cpp
 *
 *  Created on: Feb 28, 2017
 *      Author: krzysztof.kocon
 */

#include "awa/Common.hpp"
#include "awa/Consts.hpp"
#include "awa/ServerSession.hpp"
#include "awa/ServerWriteOperation.hpp"

using namespace awa;

ServerWriteOperation::ServerWriteOperation (std::string clientId,
                                            const ServerSession& session) throw (std::bad_alloc) :  clientId(clientId) {

  operation = AwaServerWriteOperation_New(session, AwaWriteMode_Update);
  if (operation == NULL) {
      throw std::bad_alloc();
  }
}

ServerWriteOperation::~ServerWriteOperation() {
  try {
      if (operation) {
          AWA_CHECK(AwaServerWriteOperation_Free(&operation));
      }
  }
  catch (const AwaException& e) {
      //skip
  }
}

void ServerWriteOperation::addValueAsCString(const char* path, const char* value) throw (AwaException) {
  AWA_CHECK(AwaServerWriteOperation_AddValueAsCString(operation, path, value));
}

void ServerWriteOperation::addValueAsBoolean(const char* path, const bool value) throw (AwaException) {
  AWA_CHECK(AwaServerWriteOperation_AddValueAsBoolean(operation, path, value));
}

void ServerWriteOperation::addValueAsInteger(const char* path, const int value) throw (AwaException) {
  AWA_CHECK(AwaServerWriteOperation_AddValueAsInteger(operation, path, value));
}

void ServerWriteOperation::perform() throw (AwaException) {
  AWA_CHECK(AwaServerWriteOperation_Perform(operation, clientId.c_str(), AWA_OPERATION_PERFORM_TIMEOUT));
}
