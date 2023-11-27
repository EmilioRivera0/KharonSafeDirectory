#ifndef CONTROL_DIR_H
#define CONTROL_DIR_H

// necessary libraries -------->
#include "server_macros.h"
#include "server_structs.h"
#include "server_exceptions.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include <cstring>
#include <cerrno>

#include <iostream>
#include <vector>

// functions declaration -------->
void control_directory(const char*, std::vector<struct file_info*>&);
void create_log_file(void);

#endif // !CONTROL_DIR_H
