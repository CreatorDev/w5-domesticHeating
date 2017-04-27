/*
 * ServerSession.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: krzysztof.kocon
 */

#include "awa/Common.hpp"
#include "awa/Consts.hpp"
#include "awa/ServerSession.hpp"

using namespace awa;


ServerSession::ServerSession() throw (std::bad_alloc) {
  session = AwaServerSession_New();
  if (session == NULL) {
      throw std::bad_alloc();
  }
}

ServerSession::~ServerSession () {
  if (session) {
      try {
          AwaServerSession_Disconnect(session);
          AwaServerSession_Free(&session);
      }
      catch(const AwaException& e) {
          //skip
      }
  }
}

ServerSession::operator AwaServerSession*() const {
  return this->session;
}

void ServerSession::setIPCAsUDP(std::string address, unsigned short port) {
  AWA_CHECK(AwaServerSession_SetIPCAsUDP(session, address.c_str(), port));
}

void ServerSession::connect() throw (AwaException) {
  AWA_CHECK(AwaServerSession_Connect(session));
}
void ServerSession::process() throw (AwaException) {
  AWA_CHECK(AwaServerSession_Process(session, AWA_OPERATION_PERFORM_TIMEOUT));
}

void ServerSession::dispatchCallbacks() throw (AwaException) {
  AWA_CHECK(AwaServerSession_DispatchCallbacks(session));
}

