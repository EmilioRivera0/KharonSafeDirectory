#ifndef CLIENT_MACROS_H
#define CLIENT_MACROS_H

// necessary libraries -------->
#include <cstdlib>

// macro definition -------->
// operation error
#define ERROR -1
// program error
#define PROGRAM_ERROR exit(EXIT_FAILURE);

// buffer max length
#define BUFFER_LENGTH 100

// semaphore names
#define CLIENT_WAIT_SEMAPHORE "/KSDcwas"
// semaphore flags
#define SEMAPHORE_FLAGS 0

// FIFO path
#define COMMUNICATION_FIFO "/dev/shm/KSDcf"
// FIFO flags
#define FIFO_FLAGS O_WRONLY|O_ASYNC

#endif // !CLIENT_MACROS_H
