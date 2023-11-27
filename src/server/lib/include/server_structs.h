#ifndef SERVER_STRUCTS_H
#define SERVER_STRUCTS_H

// necessary libraries -------->
#include <sys/stat.h>

// structs declaration -------->
struct file_info{
  char* file_name;
  char* absolute_path;
  char* semaphore;
};

struct thread_parameters{
  char* path;
  uid_t uid;
  gid_t gid;
  mode_t permissions;
  char* semaphore;
};

#endif // !SERVER_STRUCTS_H
