#ifndef SERVER_STRUCTS_H
#define SERVER_STRUCTS_H

// necessary libraries -------->
#include <semaphore.h>
#include <sys/stat.h>
#include <vector>

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
  short mode;
};

struct close_server_thread_parameters{
  const std::vector<struct file_info*>* dir_entrys;
  sem_t* cwait_sem;
  int cpfd;
};

#endif // !SERVER_STRUCTS_H
