// necessary libraries -------->
#include "../include/connection.h"

// functions definition -------->
void get_dir_entrys(std::vector<std::string>& dir_entrys){
  // local variables
  char * token{nullptr};
  char buffer[BUFFER_LENGTH]{};
  int shm_fd{0};
  // clean buffer content
  memset(buffer, '\0', BUFFER_LENGTH);

  // open shm
  shm_fd = shm_open(SHM, SHM_FLAGS, SHM_PERMISSIONS);
  if (shm_fd == ERROR) {
    std::cout << "\nCould not create SHM " << SHM << ": ";
    throw exc::ClientException(strerror(errno));
  }
  // write shm content into local buffer
  read(shm_fd, buffer, BUFFER_LENGTH);
  // store each directory entry inside the dir_entrys vector to display them to the user
  token = strtok(buffer, "-");
  while (token) {
    dir_entrys.push_back(token);
    token = strtok(nullptr, "-");
  }
  // close shm
  close(shm_fd);
}
