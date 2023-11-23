#ifndef CONTROL_DIR_H
#define CONTROL_DIR_H

// necessary libraries -------->
#include "server_macros.h"
#include "server_structs.h"
#include "server_exceptions.h"

#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include <cstring>
#include <cerrno>

#include <iostream>
#include <vector>

// functions declaration -------->
void control_directory(const char*, std::vector<struct file_path*>&);

#endif // !CONTROL_DIR_H
