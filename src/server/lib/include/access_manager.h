#ifndef ACCESS_MANAGER_H
#define ACCESS_MANAGER_H

// necessary libraries -------->
#include "../include/server_structs.h"
#include "../include/server_macros.h"
#include "../include/server_exceptions.h"

#include <semaphore.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>

#include <cstring>
#include <cerrno>
#include <ctime>

#include <iostream>
#include <fstream>

// functions declaration -------->
void access_manager(const std::vector<struct file_info*>*);
void create_semaphores(const std::vector<struct file_info*>*);
void unlink_semaphores(const std::vector<struct file_info*>*);
void free_memory(const std::vector<struct file_info*> *);
void* grant_access(void*);
void* revoke_access(void*);
void* poweroff(void*);

#endif // !ACCESS_MANAGER_H
