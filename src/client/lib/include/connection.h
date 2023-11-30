#ifndef CONNECTION_H
#define CONNECTION_H

// necessary libraries -------->
#include "client_exceptions.h"
#include "client_macros.h"

#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <cstring>
#include <vector>
#include <string>

// functions declaration -------->
void get_dir_entrys(std::vector<std::string>&);

#endif // !CONNECTION_H
