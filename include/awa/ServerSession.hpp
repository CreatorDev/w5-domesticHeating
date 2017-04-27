/*
 * ServerSession.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _AWA_SERVERSESSION_HPP_
#define _AWA_SERVERSESSION_HPP_

#include <stdexcept>
#include <awa/server.h>
#include "awa/AwaException.hpp"


namespace awa
{

  class ServerSession
  {
    AwaServerSession* session;
  public:
    ServerSession() throw (std::bad_alloc);
    virtual ~ServerSession ();

    operator AwaServerSession*() const;

    void setIPCAsUDP(std::string address, unsigned short port);
    void connect() throw (AwaException);
    void process() throw (AwaException);
    void dispatchCallbacks() throw (AwaException);
  };

} /* namespace awa */

#endif /* _AWA_SERVERSESSION_HPP_ */
