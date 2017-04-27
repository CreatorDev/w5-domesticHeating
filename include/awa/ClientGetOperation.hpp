/*
 * ClientGetOperation.h
 *
 *  Created on: Feb 22, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _CLIENTGETOPERATION_HPP_
#define _CLIENTGETOPERATION_HPP_

#include <stdexcept>
#include <memory>

#include <awa/client.h>

#include "awa/AwaException.hpp"


namespace awa
{

  class ClientSession;

  class ClientGetOperation
  {
     AwaClientGetOperation* operation;
  public:
    ClientGetOperation(const ClientSession& session) throw (std::bad_alloc);

    void addPath(const char* resourcePath) throw (AwaException);
    void perform() throw (AwaException);

    operator AwaClientGetOperation*() const;

    virtual ~ClientGetOperation();
  };

} /* namespace awa */

#endif /* _CLIENTGETOPERATION_HPP_ */
