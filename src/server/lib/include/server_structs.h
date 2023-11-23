#ifndef SERVER_STRUCTS_H
#define SERVER_STRUCTS_H

// structs declaration -------->
struct file_path{
  char* file_name;
  char* absolute_path;
  struct file_path* next;
};

#endif // !SERVER_STRUCTS_H
