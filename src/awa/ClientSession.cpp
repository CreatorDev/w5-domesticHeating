/*
 * AwaClientSession.cpp
 *
 *  Created on: Feb 8, 2017
 *      Author: krzysztof.kocon
 */

#include "awa/AwaException.hpp"
#include "awa/Common.hpp"
#include "awa/Consts.hpp"
#include "awa/ClientSession.hpp"

using namespace awa;


ClientSession::ClientSession() throw (std::bad_alloc) {
  session = AwaClientSession_New();
  if (session == NULL) {
      throw std::bad_alloc();
  }
}

ClientSession::~ClientSession() {
  if (session) {
      try {
          AwaClientSession_Disconnect(session);
          AwaClientSession_Free(&session);
      }
      catch(const AwaException& e) {
          //skip
      }
  }
}

ClientSession::operator AwaClientSession*() const {
  return session;
}

void ClientSession::connect() throw (AwaException) {
  AWA_CHECK(AwaClientSession_Connect(session));
}


void ClientSession::process() throw (AwaException) {
    AWA_CHECK(AwaClientSession_Process(session, AWA_OPERATION_PERFORM_TIMEOUT));
}

void ClientSession::dispatchCallbacks() throw (AwaException) {
    AWA_CHECK(AwaClientSession_DispatchCallbacks(session));
}
