// necessary libraries -------->
#include "../include/server_exceptions.h"

// class methods definition -------->
exc::ServerException::ServerException(){}
exc::ServerException::~ServerException(){}
exc::ServerException::ServerException(const char* cstring) : message{cstring}{}
const char* exc::ServerException::what() const noexcept{
  return this->message;
}
