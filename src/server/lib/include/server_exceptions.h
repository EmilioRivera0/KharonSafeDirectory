#ifndef SERVER_EXCEPTION_H
#define SERVER_EXCEPTION_H

// necessary libraries -------->
#include <exception>

// exceptions declaration -------->
namespace exc{
class ServerException : std::exception{
public:
  ServerException();
  ~ServerException();
  ServerException(const char*);
  virtual const char* what() const noexcept;
private:
  const char* message;
};
}

#endif // !SERVER_EXCEPTION_H
