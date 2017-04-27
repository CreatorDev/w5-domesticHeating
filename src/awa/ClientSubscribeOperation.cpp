/*
 * ClientSubscribeOperation.cpp
 *
 *  Created on: Feb 8, 2017
 *      Author: krzysztof.kocon
 */

#include "awa/AwaException.hpp"
#include "awa/Common.hpp"
#include "awa/Consts.hpp"
#include "awa/ClientSession.hpp"
#include "awa/ClientChangeSubscription.hpp"
#include "awa/ClientSubscribeOperation.hpp"

using namespace awa;


ClientSubscribeOperation::ClientSubscribeOperation(const ClientSession& session) throw (std::bad_alloc) {
  subscribeOperation = AwaClientSubscribeOperation_New(session);
  if (subscribeOperation == NULL) {
      throw std::bad_alloc();
  }
}

ClientSubscribeOperation::~ClientSubscribeOperation() {
  if (subscribeOperation) {
      try {
          AWA_CHECK(AwaClientSubscribeOperation_Free(&subscribeOperation));
      }
      catch(const AwaException& e) {
        //skip
      }
  }
}

void ClientSubscribeOperation::addChangeSubscription(std::shared_ptr<awa::ClientChangeSubscription> subscription) throw (AwaException) {
  AWA_CHECK(AwaClientSubscribeOperation_AddChangeSubscription(subscribeOperation, *subscription));
}

void ClientSubscribeOperation::addCancelChangeSubscription(std::shared_ptr<awa::ClientChangeSubscription> subscription) throw (AwaException) {
  AWA_CHECK(AwaClientSubscribeOperation_AddCancelChangeSubscription(subscribeOperation, *subscription));
}

void ClientSubscribeOperation::perform() throw (AwaException) {
  AWA_CHECK(AwaClientSubscribeOperation_Perform(subscribeOperation, AWA_OPERATION_PERFORM_TIMEOUT));
}
