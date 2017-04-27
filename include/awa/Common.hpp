/*
 * Common.hpp
 *
 *  Created on: Feb 8, 2017
 *      Author: krzysztof.kocon
 */

#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#define AWA_CHECK(x)    \
  AwaError err = (x);   \
  if (err != AwaError_Success) {           \
      throw awa::AwaException(err);        \
  }                                        \

#endif
