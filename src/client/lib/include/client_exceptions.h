#ifndef CLIENT_EXCEPTION_H
#define CLIENT_EXCEPTION_H

// necessary libraries -------->
#include <exception>

// exceptions declaration -------->
namespace exc{
class ClientException : std::exception{
public:
  ClientException();
  ~ClientException();
  ClientException(const char*);
  virtual const char* what() const noexcept;
private:
  const char* message;
};
}

#endif // !CLIENT_EXCEPTION_H
