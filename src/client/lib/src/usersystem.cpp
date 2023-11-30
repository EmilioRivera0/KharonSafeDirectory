#include "../include/usersystem.h"
#include "../include/client_macros.h"
#include "../include/client_exceptions.h"

//Functions ---------->
UserSystem::UserSystem(const std::vector<std::string>& de) : dir_entrys{de} {
  // open communication pipe to send data to the server
  this->pipe_fd = open(COMMUNICATION_FIFO, FIFO_FLAGS);
  if (this->pipe_fd == ERROR) {
    std::cout << "\nCould not open " << COMMUNICATION_FIFO << " FIFO: ";
    throw exc::ClientException(strerror(errno));
  }
 
  // create a semaphore to synchronize with the server process before accessing the selected file
  this->cwait_sem = sem_open(CLIENT_WAIT_SEMAPHORE, SEMAPHORE_FLAGS);
  // check if semaphore was opened successfully
  if (this->cwait_sem == SEM_FAILED) {
    std::cout << "\nCould not open " << CLIENT_WAIT_SEMAPHORE << " semaphore: ";
    throw exc::ClientException(strerror(errno));
  }
}

int UserSystem::principal_menu() {
    int option;
    std::cout << "\n\tWELCOME TO KHARON SAVE DIRECTORY" << std::endl;
    std::cout << "1. Sign in" << std::endl;
    std::cout << "2. Sign up" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Option: ";
    std::cin >> option;
    std::cin.get();
    return option;
}

bool UserSystem::sign_in() { 
    int option;
    std::string name, password;
    do {
        std::cout << "--------------------" << std::endl;
        std::cout << "\tSIGNIN" << std::endl;
        std::cout << "--------------------" << std::endl;
        std::cout << "User: ";
        getline(std::cin, name);
        std::cout << "Password: ";
        getline(std::cin, password);
        if (this->verify_user(name, password, "i")) {
            this->error_message = "";
            return true;
        }
        else {
            std::cout << this->error_message << std::endl;
            std::cout << "0: exit - 1: continue > ";
            std::cin >> option;
        }
        std::cin.get();
    } while(option != 0);

    this->error_message = "";
    return false;
}

bool UserSystem::sign_up() { 
    int option, age;
    std::string name, password, email;
    do {
        std::cout << "--------------------" << std::endl;
        std::cout << "\tSIGNUP" << std::endl;
        std::cout << "--------------------" << std::endl;
        std::cout << "User: ";
        getline(std::cin, name);
        std::cout << "Password: ";
        getline(std::cin, password);
        std::cout << "Age: ";
        std::cin >> age;
        std::cout << "Email: ";
        std::cin >> email;
        if (this->verify_user(name, password, "u")) {
            this->current_user = create_user(name, password, age, email, "user");
            this->error_message = "";
            return true;
        }
        else {
            std::cout << this->error_message << std::endl;
            std::cout << "0: exit - 1: continue > ";
            std::cin >> option;
        }
        std::cin.get();
    } while(option != 0);

    this->error_message = "";
    return false;
}

bool UserSystem::verify_user(std:: string name, std::string password, std::string type) {
    if (strcmp(type.c_str(), "i") == 0) {
        for (int i = 0; i < users.size(); i++) {
            if (strcmp(name.c_str(), this->users[i].get_name().c_str()) == 0 && strcmp(password.c_str(), this->users[i].get_password().c_str()) == 0) {
                this->current_user = this->users[i];
                return true;
            }
        }
        this->error_message = "USER NOT FOUND";
        return false;
    }
    else {
        for (int i = 0; i < users.size(); i++) {
            if (strcmp(name.c_str(), this->users[i].get_name().c_str()) == 0) {
                this->error_message = "USER ALREADY EXISTS";
                return false;
            }
        }
        return true;
    }
    return false;
}

User UserSystem::get_current_user() {
    return this->current_user;
}

User UserSystem::create_user(std::string name, std::string password, int age, std::string email, std::string type) {
    User user = User(name, password, age, email, type);
    this->users.push_back(user);
    return user;
}

int UserSystem::loggedin_menu() {
    int option;
    std::cout << "\t" << this->current_user.get_name() << std::endl;
    std::cout << "1. Write file" << std::endl;
    std::cout << "2. Read file" << std::endl;
    std::cout << "3. Read and Write file" << std::endl;
    std::cout << "4. View account" << std::endl;
    std::cout << "0. Sign out" << std::endl;
    std::cout << "Option: ";
    std::cin >> option;
    std::cin.get();
    return option;
}

void UserSystem::access_file(short mode, short index, uid_t uid){
  // local variables
  short op{0};
  sem_t *file_sem{nullptr};
  std::string message{}, semaphore{"/"};
  // open file semaphore
  semaphore += this->dir_entrys[index];
  file_sem = sem_open(semaphore.c_str(), SEMAPHORE_FLAGS);
  if (file_sem == SEM_FAILED) {
    std::cout << "\nCould not open " << semaphore << " semaphore: ";
    throw exc::ClientException(strerror(errno));
  }
  // generate message
  message = std::to_string(mode) + "-" + std::to_string(index) + "-" + std::to_string(uid);
  // send message to server
  write(this->pipe_fd, message.c_str(), message.size());
  // wait for server to process message and grant access to the file
  sem_wait(this->cwait_sem);
  usleep(100);
  // continue to taking control of the file
  sem_post(this->cwait_sem);
  usleep(100);
  sem_wait(file_sem);
  // wait for user input to release file control
  std::cout << "1. Release File\nOp-> ";
  std::cin >> op;
  sem_post(file_sem);
}

short UserSystem::get_index(){
  // local variables
  short index{0};
  // show user directory entrys
  std::cout << "\nFiles" << std::endl;
  for (short it = 1; it <= this->dir_entrys.size(); it++)
    std::cout << it << ". " << dir_entrys[it-1] << std::endl;
  std::cout << "\tOption-> ";
  std::cin >> index;

  return index-1;
}

uid_t UserSystem::get_uid(){
  // local variables
  std::ifstream passwd{};
  uid_t uid{0};
  std::string username{};
  char buffer[BUFFER_LENGTH], *found{nullptr}, *token{nullptr};
  do {
    // open file
    passwd.open("/etc/passwd");
    if (!passwd.is_open()) 
      throw exc::ClientException("Could not open /etc/passwd file");
    // ask user for system username
    std::cout << "Enter system username: ";
    std::cin >> username;
    // search for user through passwd file
    while (!passwd.eof()) {
      passwd.getline(buffer, BUFFER_LENGTH);
      found = strstr(buffer, username.c_str());
      // user found
      if (found != nullptr) {
        // username
        token = strtok(buffer, ":");
        // "password"
        token = strtok(nullptr, ":");
        // uid
        token = strtok(nullptr, ":");
        return atoi(token);
      }
    }
    std::cout << "Invalid username." << std::endl;
    passwd.close();
  } while(true);
  // ask user for system username

  return uid;
}
