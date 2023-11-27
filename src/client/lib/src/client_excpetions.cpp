// necessary libraries -------->
#include "../include/client_exceptions.h"

// class methods definition -------->
exc::ClientException::ClientException(){}
exc::ClientException::~ClientException(){}
exc::ClientException::ClientException(const char* cstring) : message{cstring}{}
const char* exc::ClientException::what() const noexcept{
  return this->message;
}
