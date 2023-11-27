#ifndef SERVER_MACROS_H
#define SERVER_MACROS_H

// necessary libraries -------->
#include <sys/stat.h>
#include <fcntl.h>

#include <cstdlib>

// macro definition -------->
// operation error
#define ERROR -1
// program error
#define PROGRAM_ERROR exit(EXIT_FAILURE);

// file permissions
#define DIR_PERMISSIONS S_IROTH|S_IXOTH
#define BASE_PERMISSIONS 0
#define ENABLE_READING BASE_PERMISSIONS|S_IRUSR
#define ENABLE_WRITING BASE_PERMISSIONS|S_IWUSR
#define ENABLE_READWRIT BASE_PERMISSIONS|S_IRUSR|S_IWUSR

// root uid & gid
#define ROOT_UID 0
#define ROOT_GID 0

// buffer max length
#define BUFFER_LENGTH 100

// semaphore names
#define CLIENT_WAIT_SEMAPHORE "/KSDcwas"
// semaphore permissions
#define SEMAPHORE_PERMISSIONS S_IRWXU
// semaphore flags
#define SEMAPHORE_FLAGS O_CREAT|O_EXCL
#define THREAD_SEMAPHORE_FLAGS 0
// semaphore initial value
#define WAIT_SEMAPHORE_INITIAL_VALUE 0
#define FILE_SEMAPHORE_INITIAL_VALUE 1

// FIFO path
#define COMMUNICATION_FIFO "/dev/shm/KSDcf"
// FIFO permissions
#define FIFO_PERMISSIONS S_IRWXU
// FIFO flags
#define FIFO_FLAGS O_RDONLY|O_ASYNC

// log file
#define LOG_FILE "Kharon.log"
#define LOG_FILE_FLAGS O_CREAT
#define LOG_FILE_PERMISSION S_IRWXU

// shm
#define SHM "/DirEntrys"
#define SHM_FLAGS O_CREAT|O_EXCL|O_RDWR
#define SHM_PERMISSIONS S_IRWXU

#endif // !SERVER_MACROS_H
