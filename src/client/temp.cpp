// necessary libraries -------->
#include "lib/include/client_macros.h"
#include "lib/include/client_exceptions.h"
#include <fcntl.h>
#include <semaphore.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h>

// server execution starting point -------->
int main (int argc, char **argv) {
  // main function variables
  char buffer[BUFFER_LENGTH];
  sem_t *cwait_sem{nullptr};
  sem_t *file_sem{nullptr};
  int p{0};
  try {
    cwait_sem = sem_open(CLIENT_WAIT_SEMAPHORE, SEMAPHORE_FLAGS);
    file_sem = sem_open("/f2.csv", SEMAPHORE_FLAGS);
    // check if semaphore was opened successfully
    if (file_sem == SEM_FAILED) {
      std::cout << "\nCould not open " << "/f2.csv" << " semaphore: ";
      throw exc::ClientException(strerror(errno));
    }
    std::cout << "Opening FIFO" << std::endl;
    int fd = open(COMMUNICATION_FIFO, FIFO_FLAGS);
    if (fd == ERROR) {
      std::cout << "\nCould not open " << COMMUNICATION_FIFO << " FIFO: ";
      throw exc::ClientException(strerror(errno));
    }
    std::cout << "UID: ";
    std::cin >> buffer;
    write(fd, buffer, BUFFER_LENGTH);
    sem_wait(cwait_sem);
    usleep(1);
    sem_post(cwait_sem);
    sem_wait(file_sem);
    std::cout << "Taken control of file" << std::endl;
    std::cout << "Enter num to continiue";
    std::cin >> p;
    sem_post(file_sem);
    std::cout << "End" << std::endl;
  }
  catch (const exc::ClientException& e) {
    std::cout << e.what() << std::endl << std::endl;
    PROGRAM_ERROR
  }
  
  return 0;
}
