/*
 * ClientSubscribeOperation.hpp
 *
 *  Created on: Feb 8, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _CLIENTSUBSCRIBEOPERATION_HPP_
#define _CLIENTSUBSCRIBEOPERATION_HPP_

#include <memory>
#include <awa/client.h>

#include "awa/AwaException.hpp"

namespace awa
{

  class ClientSession;
  class ClientChangeSubscription;

  class ClientSubscribeOperation
  {
    AwaClientSubscribeOperation* subscribeOperation;
  public:
    ClientSubscribeOperation(const ClientSession& session) throw (std::bad_alloc);
    virtual ~ClientSubscribeOperation();

    void addChangeSubscription(std::shared_ptr<awa::ClientChangeSubscription> subscription) throw (AwaException);
    void addCancelChangeSubscription(std::shared_ptr<awa::ClientChangeSubscription> subscription) throw (AwaException);
    void perform() throw (AwaException);
  };

} /* namespace awa */

#endif /* _CLIENTSUBSCRIBEOPERATION_HPP_ */
