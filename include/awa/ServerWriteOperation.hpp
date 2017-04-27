/*
 * ServerWriteOperation.hpp
 *
 *  Created on: Feb 28, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _SERVERWRITEOPERATION_HPP_
#define _SERVERWRITEOPERATION_HPP_

#include <stdexcept>
#include <string>
#include <awa/server.h>
#include "awa/AwaException.hpp"

namespace awa
{
  class ClientSession;

  class ServerWriteOperation
  {
    std::string clientId;
    AwaServerWriteOperation * operation;

  public:
    ServerWriteOperation(std::string clientId, const ServerSession& session) throw (std::bad_alloc);
    virtual ~ServerWriteOperation ();

    void addValueAsCString(const char* path, const char* value) throw (AwaException);
    void addValueAsBoolean(const char* path, const bool value) throw (AwaException);
    void addValueAsInteger(const char* path, const int value) throw (AwaException);
    void perform() throw (AwaException);
  };

} /* namespace awa */

#endif /* _SERVERWRITEOPERATION_HPP_ */
