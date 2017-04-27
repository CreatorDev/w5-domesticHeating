/*
 * AwaException.cpp
 *
 *  Created on: Feb 13, 2017
 *      Author: krzysztof.kocon
 */

#include "awa/AwaException.hpp"
#include<string>
#include<sstream>

using namespace awa;


AwaException::AwaException(AwaError err) : std::exception(), error(err){
  std::stringstream s;
  s << "Awa failed with error code:" << error;
  message = s.str();
}

AwaException::~AwaException (){

}

const char* AwaException::what() const noexcept {

  return message.c_str();
}

int AwaException::getErrorCode() const noexcept {
  return error;
}
