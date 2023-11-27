// necessary libraries -------->
#include "../include/control_dir.h"

// functions definition -------->
void create_log_file(){
  // local variables
  int fd{open(LOG_FILE,LOG_FILE_FLAGS, LOG_FILE_PERMISSION)};
  if (fd == ERROR) {
    std::cout << "\nCould open/create " << LOG_FILE << ": ";
    throw exc::ServerException(strerror(errno));
  }
  close(fd);
}

void control_directory(const char* dir_name, std::vector<struct file_info*>& dir_entrys){
  // local variables
  short fn_len{0}, ap_len{0}, sm_len{0};
  int shm_fd{0};
  char abs_path[BUFFER_LENGTH];
  char file_semaphore[BUFFER_LENGTH];
  char format[] = "/";
  DIR* dir{nullptr};
  struct dirent* entry{nullptr};
  struct file_info* temp{nullptr};

  // create log file
  create_log_file();
  // create shm
  shm_fd = shm_open(SHM, SHM_FLAGS, SHM_PERMISSIONS);
  if (shm_fd == ERROR) {
    std::cout << "\nCould not create SHM " << SHM << ": ";
    throw exc::ServerException(strerror(errno));
  }
  if (ftruncate(shm_fd, BUFFER_LENGTH) == ERROR){
    std::cout << "\nCould not truncate SHM " << SHM << ": ";
    throw exc::ServerException(strerror(errno));
  }

  // open directory and verify if it exists or the program can interact with it
  dir = opendir(dir_name);
  if (dir == nullptr) {
    std::cout << "\nCould not open directory: ";
    throw exc::ServerException(strerror(errno));
  }
  // change owner and group of the directory
  if (chown(dir_name, ROOT_UID, ROOT_GID) == ERROR) {
    std::cout << "\nCould not change owner and group of " << dir_name << ": ";
    throw exc::ServerException(strerror(errno));
  }
  // set permissions to read, write and execute only to owner and group for the directory
  if (chmod(dir_name, DIR_PERMISSIONS) == ERROR) {
    std::cout << "\nCould not change permissions of " << dir_name << ": ";
    throw exc::ServerException(strerror(errno));
  }

  // directory opened successfully
  while ((entry = readdir(dir))) {
    // emtpy buffer content & concatenate the abolute path of the directory
    memset(abs_path, '\0', BUFFER_LENGTH);
    strcat(abs_path, dir_name);
    strcat(abs_path, format);
    // emtpy buffer content & add a '/' at 0 index
    memset(file_semaphore, '\0', BUFFER_LENGTH);
    file_semaphore[0] = '/';
    // ignore . and .. directories
    if ((strcmp(entry->d_name, "..") == 0) || (strcmp(entry->d_name, ".") == 0))
      continue;
    // concatenate file name
    strcat(abs_path, entry->d_name);
    strcat(file_semaphore, entry->d_name);
    // add by 1 to include the terminating c-string character
    fn_len = strlen(entry->d_name)+1;
    ap_len = strlen(abs_path)+1;
    sm_len = strlen(file_semaphore)+1;
    // allocate space for new file_info struct and initialize it
    temp = new struct file_info;
    temp->file_name = new char[fn_len];
    temp->absolute_path = new char[ap_len];
    temp->semaphore = new char[sm_len];
    memcpy(temp->file_name, entry->d_name, fn_len);
    memcpy(temp->absolute_path, abs_path, ap_len);
    memcpy(temp->semaphore, file_semaphore, sm_len);
    // append the new structure into the dir_entrys vector
    dir_entrys.push_back(temp);
    // change owner and group of each entry
    if (chown(abs_path, ROOT_UID, ROOT_GID) == ERROR) {
      std::cout << "\nCould not change owner and group of " << abs_path << ": ";
      throw exc::ServerException(strerror(errno));
    }
    // set permissions to read, write and execute only to owner and group of each entry
    if (chmod(abs_path, BASE_PERMISSIONS) == ERROR) {
      std::cout << "\nCould not change permissions of " << abs_path << ": ";
      throw exc::ServerException(strerror(errno));
    }
  }
  closedir(dir);
  close(shm_fd);
}
