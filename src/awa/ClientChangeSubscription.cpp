/*
 * ClientChangeSubscription.cpp
 *
 *  Created on: Feb 8, 2017
 *      Author: krzysztof.kocon
 */

#include <awa/ClientChangeSubscription.hpp>
#include "awa/AwaException.hpp"
#include "awa/Common.hpp"

using namespace awa;


ClientChangeSubscription::ClientChangeSubscription (const char * path,
                                                    AwaClientSubscribeToChangeCallback callback,
                                                    void * context) throw (std::bad_alloc) {
  subscription = AwaClientChangeSubscription_New(path, callback, context);
  if (subscription == NULL) {
      throw std::bad_alloc();
  }
}

ClientChangeSubscription::~ClientChangeSubscription () {
  try {
      if (subscription) {
          AWA_CHECK(AwaClientChangeSubscription_Free(&subscription));
      }
  }
  catch(const AwaException& e) {
      //skip
  }
}

ClientChangeSubscription::operator AwaClientChangeSubscription*() const {
  return subscription;
}
