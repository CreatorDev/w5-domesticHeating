/*
 * ClientGetResponse.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: krzysztof.kocon
 */

#include "awa/Common.hpp"
#include "awa/Consts.hpp"
#include "awa/ClientGetOperation.hpp"
#include "awa/ClientGetResponse.hpp"

using namespace awa;


ClientGetResponse::ClientGetResponse(const ClientGetOperation& operation) {
  response = AwaClientGetOperation_GetResponse(operation);
}

ClientGetResponse::~ClientGetResponse () {

}

void ClientGetResponse::getValueAsCStringPointer(const char* path, const char** value) throw (AwaException) {
  if (response) {
      AWA_CHECK(AwaClientGetResponse_GetValueAsCStringPointer(response, path, value));
  }
}

void ClientGetResponse::getValueAsBooleanPointer(const char* path, const bool** value) throw (AwaException) {
  if (response) {
      AWA_CHECK(AwaClientGetResponse_GetValueAsBooleanPointer(response, path, value));
  }
}

void ClientGetResponse::getValueAsFloatPointer(const char* path, const double** value) throw (AwaException) {
  if (response) {
      AWA_CHECK(AwaClientGetResponse_GetValueAsFloatPointer(response, path, value));
  }
}
