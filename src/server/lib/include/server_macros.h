#ifndef SERVER_MACROS_H
#define SERVER_MACROS_H

// necessary libraries -------->
#include <sys/stat.h>
#include <cstdlib>

// macro definition -------->
// file permissions
#define BASE_PERMISSIONS S_IRWXU|S_IRWXG
#define ENABLE_READING BASE_PERMISSIONS|S_IROTH
#define ENABLE_WRITING BASE_PERMISSIONS|S_IWOTH
// directory name length
#define DIR_NAME_MAX_LEN 50
// program error
#define PROGRAM_ERROR exit(EXIT_FAILURE);
// root uid & gid
#define ROOT_UID 0
#define ROOT_GID 0
// buffer max length
#define BUFFER_LENGTH 100

#endif // !SERVER_MACROS_H
