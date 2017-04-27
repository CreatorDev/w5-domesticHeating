/*
 * ServerObservation.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _AWA_SERVEROBSERVATION_HPP_
#define _AWA_SERVEROBSERVATION_HPP_

#include <stdexcept>
#include <string>
#include <awa/server.h>
#include "awa/AwaException.hpp"


namespace awa
{

  class ServerObservation
  {
    AwaServerObservation * observation;

  public:
    ServerObservation(std::string clientId,
                      std::string resourcePath,
                      AwaServerObservationCallback callback,
                      void* context);
    virtual ~ServerObservation ();

    operator AwaServerObservation*() const;
  };

} /* namespace awa */

#endif /* INCLUDE_AWA_SERVEROBSERVATION_HPP_ */
