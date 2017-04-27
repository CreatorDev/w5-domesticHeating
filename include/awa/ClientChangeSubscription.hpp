/*
 * ClientChangeSubscription.hpp
 *
 *  Created on: Feb 8, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _CLIENTCHANGESUBSCRIPTION_HPP_
#define _CLIENTCHANGESUBSCRIPTION_HPP_

#include <awa/client.h>
#include <stdexcept>

namespace awa
{

  class ClientChangeSubscription
  {
    AwaClientChangeSubscription*    subscription;
  public:
    ClientChangeSubscription(const char * path,
                             AwaClientSubscribeToChangeCallback callback,
                             void * context) throw (std::bad_alloc);

    operator AwaClientChangeSubscription*() const;
    virtual ~ClientChangeSubscription();
  };

} /* namespace awa */

#endif /* _CLIENTCHANGESUBSCRIPTION_HPP_ */
