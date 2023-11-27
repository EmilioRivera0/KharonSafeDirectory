// necessary libraries -------->
#include "../include/access_manager.h"

// functions definition -------->
void create_semaphores(const std::vector<struct file_info*> &dir_entrys){
  // local variables
  sem_t* temp{nullptr};
  for (const auto it : dir_entrys){
    // create a file semaphore to synchronize operations related to each file
    temp = sem_open(it->semaphore, SEMAPHORE_FLAGS, SEMAPHORE_PERMISSIONS, FILE_SEMAPHORE_INITIAL_VALUE);
    // check if semaphore was opened successfully
    if (temp == SEM_FAILED) {
      std::cout << "\nCould not create " << it->semaphore << " semaphore: ";
      throw exc::ServerException(strerror(errno));
    }
    sem_close(temp);
  }
}

void unlink_semaphores(const std::vector<struct file_info*> &dir_entrys){
  // unlink all file semaphores
  for (const auto it : dir_entrys){
    sem_unlink(it->semaphore);
  }
}

void* grant_access(void* args){
  // local variables
  sem_t *file_sem{nullptr};
  time_t raw;
  struct thread_parameters* tp{(struct thread_parameters*)args};
  std::ofstream log(LOG_FILE, std::ofstream::app);
  // make thread independent
  pthread_detach(pthread_self());
  try {
    // create a semaphore to synchronize with the client process before accessing the selected file
    file_sem = sem_open(tp->semaphore, THREAD_SEMAPHORE_FLAGS);
    // check if semaphore was opened successfully
    if (file_sem == SEM_FAILED) {
      std::cout << "\nThread could not open " << tp->semaphore << " semaphore: ";
      throw exc::ServerException(strerror(errno));
    }
  
    sem_wait(file_sem);
  
    // change ownership to specified user and group
    if (chown(tp->path, tp->uid, tp->gid) == ERROR) {
      std::cout << "\nThread could not change owner and group of " << tp->path << ": ";
      throw exc::ServerException(strerror(errno));
    }
    // change permissions of the enabled file
    if (chmod(tp->path, tp->permissions) == ERROR) {
      std::cout << "\nThread could not change permissions of " << tp->path << ": ";
      throw exc::ServerException(strerror(errno));
    }
    // get current time for logging
    time(&raw);
    // log action
    log << "->" << asctime(localtime(&raw)) << "\t" << tp->path << " access granted to UID: " << tp->uid << std::endl;
    
    // release file semaphore, log file and terminate thread
    sem_post(file_sem);
    sem_close(file_sem);
    log.close();
    pthread_exit(nullptr);
  }
  catch (const exc::ServerException& e) {
    std::cout << e.what() << std::endl << std::endl;
    PROGRAM_ERROR
  }
  return nullptr;
}

void* revoke_access(void* args){
  // local variables
  sem_t *file_sem{nullptr};
  time_t raw;
  struct thread_parameters* tp{(struct thread_parameters*)args};
  std::ofstream log(LOG_FILE, std::ofstream::app);
  // make thread independent
  pthread_detach(pthread_self());
  try {
    // create a semaphore to synchronize with the client process before accessing the selected file
    file_sem = sem_open(tp->semaphore, THREAD_SEMAPHORE_FLAGS);
    // check if semaphore was opened successfully
    if (file_sem == SEM_FAILED) {
      std::cout << "\nThread could not open " << tp->semaphore << " semaphore: ";
      throw exc::ServerException(strerror(errno));
    }
  
    sem_wait(file_sem);
    
    // change ownership to root user and group
    if (chown(tp->path, ROOT_UID, ROOT_GID) == ERROR) {
      std::cout << "\nCould not change owner and group of " << tp->path << ": ";
      throw exc::ServerException(strerror(errno));
    }
    // remove permissions of the enabled file
    if (chmod(tp->path, BASE_PERMISSIONS) == ERROR) {
      std::cout << "\nCould not change permissions of " << tp->path << ": ";
      throw exc::ServerException(strerror(errno));
    }
    // get current time for logging
    time(&raw);
    // log action
    log << "->" << asctime(localtime(&raw)) << "\t" << tp->path << " access revoked to UID: " << tp->uid << std::endl;
  
    // release file semaphore, log file and terminate thread
    sem_post(file_sem);
    sem_close(file_sem);
    log.close();
    // remove allocated thread parameters struct from heap memory since it will not be used again
    delete[] tp->path;
    delete[] tp->semaphore;
    delete tp;
    pthread_exit(nullptr);
  }
  catch (const exc::ServerException& e) {
    std::cout << e.what() << std::endl << std::endl;
    PROGRAM_ERROR
  } 
  return nullptr;
}

void access_manager(const std::vector<struct file_info*>& dir_entrys){
  // local variables
  short cpfd{0}, count{0}, ap_len{0}, sm_len{0};
  char buffer[BUFFER_LENGTH]{};
  sem_t *cwait_sem{nullptr};
  pthread_t thread{};
  struct thread_parameters *tp{};

  // create a semaphore to synchronize with the client process before accessing the selected file
  cwait_sem = sem_open(CLIENT_WAIT_SEMAPHORE, SEMAPHORE_FLAGS, SEMAPHORE_PERMISSIONS, WAIT_SEMAPHORE_INITIAL_VALUE);
  // check if semaphore was opened successfully
  if (cwait_sem == SEM_FAILED) {
    std::cout << "\nCould not open " << CLIENT_WAIT_SEMAPHORE << " semaphore: ";
    throw exc::ServerException(strerror(errno));
  }
  // create the semaphores to control the access to each file
  create_semaphores(dir_entrys);
  // create communication fifo
  if (mkfifo(COMMUNICATION_FIFO, FIFO_PERMISSIONS) == ERROR) {
    std::cout << "\nCould not create " << COMMUNICATION_FIFO << " FIFO: ";
    throw exc::ServerException(strerror(errno));
  }
  // at least one client needs to open the FIFO for the process to continiue its execution
  cpfd = open(COMMUNICATION_FIFO, FIFO_FLAGS);
  if (cpfd == ERROR) {
    std::cout << "\nCould not open " << COMMUNICATION_FIFO << " FIFO: ";
    throw exc::ServerException(strerror(errno));
  }

  // loop to listen and process incoming client requests
  do {
    // wait for a new message
    count = read(cpfd, buffer, BUFFER_LENGTH);
    if (count == 0) {
      continue;
    }
    else if (count == ERROR) {
      std::cout << "\nCould not read from " << COMMUNICATION_FIFO << " FIFO: ";
      throw exc::ServerException(strerror(errno));
    }
    else{
      std::cout << "New Message: " << buffer << std::endl;
      // thread parameters struct need to be located on the heap for it to be accessed by threads
      // allocate heap memory space for thread parameters struct and initialize it
      tp = new struct thread_parameters;
      ap_len = strlen(dir_entrys[0]->absolute_path)+1;
      sm_len = strlen(dir_entrys[0]->semaphore)+1;
      tp->path = new char[ap_len];
      tp->semaphore = new char[sm_len];
      memcpy(tp->path, dir_entrys[0]->absolute_path, ap_len);
      memcpy(tp->semaphore, dir_entrys[0]->semaphore, sm_len);
      tp->permissions = ENABLE_READWRIT;
      tp->uid = 1000;
      tp->gid = ROOT_GID;
      // create first thread to grant the requesting client access to the specified file
      usleep(1000);
      pthread_create(&thread, NULL, &grant_access, (void*)tp);
      // release the client from the wait semaphore to give him the control over the file
      sem_post(cwait_sem);
      usleep(1000);
      // take control of the wait semaphore to prevent future requesting clients to access any file without it being enabled
      sem_wait(cwait_sem);
      usleep(1000);
      // create second thread to revoke the permissions and ownership to the client
      // this prevents the same client accessing the file in the future without requesting access again
      pthread_create(&thread, NULL, &revoke_access, (void*)tp);
    }
  } while(true);

  // close & unlink semaphores
  sem_close(cwait_sem);
  close(cpfd);
  sem_unlink(CLIENT_WAIT_SEMAPHORE);
  unlink(COMMUNICATION_FIFO);
  unlink_semaphores(dir_entrys);
}
