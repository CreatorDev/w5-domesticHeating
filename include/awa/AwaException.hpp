/*
 * AwaException.hpp
 *
 *  Created on: Feb 13, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _AWAEXCEPTION_HPP_
#define _AWAEXCEPTION_HPP_

#include <stdexcept>
#include <string>
#include <awa/error.h>

namespace awa
{

  class AwaException: std::exception
  {

    AwaError error;
    std::string message;
  public:
    AwaException(AwaError err);
    virtual ~AwaException();

    const char* what() const noexcept;
    int getErrorCode() const noexcept;

    typedef enum {
         NotDefined = 7,
         Timeout = 13,
         Response = 29
       } Error;
  };

} /* namespace awa */

#endif /* _AWAEXCEPTION_HPP_ */
