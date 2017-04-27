/*
 * ClientGetResponse.hpp
 *
 *  Created on: Feb 22, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _CLIENTGETRESPONSE_HPP_
#define _CLIENTGETRESPONSE_HPP_

#include <stdexcept>
#include <memory>

#include <awa/client.h>

#include "awa/AwaException.hpp"


namespace awa
{

  class ClientGetOperation;

  class ClientGetResponse
  {
    const AwaClientGetResponse * response;
  public:
    ClientGetResponse(const ClientGetOperation& operation);
    virtual ~ClientGetResponse();

    void getValueAsCStringPointer(const char* path, const char** value) throw (AwaException);
    void getValueAsBooleanPointer(const char* path, const bool** value) throw (AwaException);
    void getValueAsFloatPointer(const char* path, const double** value) throw (AwaException);
  };

} /* namespace awa */

#endif /* _CLIENTGETRESPONSE_HPP_ */
