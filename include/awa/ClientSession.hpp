/*
 * AwaClientSession.hpp
 *
 *  Created on: Feb 8, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _AWA_CLIENTSESSION_H_
#define _AWA_CLIENTSESSION_H_

#include <stdexcept>
#include <awa/client.h>
#include "awa/AwaException.hpp"

namespace awa
{

  class ClientSession
  {
    AwaClientSession* session;
  public:
    ClientSession() throw (std::bad_alloc);
    virtual ~ClientSession();

    operator AwaClientSession*() const;

    void connect() throw (AwaException);
    void process() throw (AwaException);
    void dispatchCallbacks() throw (AwaException);
  };

} /* namespace awa */

#endif /* _CLIENTSESSION_H_ */
