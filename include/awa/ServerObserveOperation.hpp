/*
 * ServerObserveOperation.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _AWA_SERVEROBSERVEOPERATION_HPP_
#define _AWA_SERVEROBSERVEOPERATION_HPP_

#include <stdexcept>
#include <awa/server.h>
#include "awa/AwaException.hpp"


namespace awa
{
  class ServerSession;
  class ServerObservation;

  class ServerObserveOperation
  {
    AwaServerObserveOperation* operation;
  public:
    ServerObserveOperation(const ServerSession& session) throw (std::bad_alloc);
    virtual ~ServerObserveOperation();

    void addObservation(const ServerObservation&) throw (AwaException);
    void addCancelObservation(const ServerObservation&) throw (AwaException);
    void perform() throw (AwaException);
  };

} /* namespace awa */

#endif /* _AWA_SERVEROBSERVEOPERATION_HPP_ */
