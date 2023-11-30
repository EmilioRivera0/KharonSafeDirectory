// necessary libraries -------->
#include "lib/include/client_exceptions.h"
#include "lib/include/client_macros.h"
#include "lib/include/usersystem.h"
#include "lib/include/connection.h"

#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <cerrno>
#include <cstring>

// server execution starting point -------->
int main (int argc, char **argv) {
  // main function variables
  std::vector<std::string> dir_entrys{};
  short option{0}, index{0};
  bool isUser{false}, exit{false};
  uid_t uid{0};
  
  // program execution starting point
  try {
    get_dir_entrys(dir_entrys);
    UserSystem us = UserSystem(dir_entrys);
    do {
      do {
        option = us.principal_menu();
        switch (option)
        {
          case 0:
            exit = true;
            break;
          case 1:
            isUser = us.sign_in();
            break;
          case 2:
            isUser = us.sign_up();
            break;
        }
      } while (!exit && !isUser);
  
      while (isUser) {
        option = us.loggedin_menu();
        switch (option)
        {
          case 0:
            isUser = false;
            break;
          // modes:
          //  0 = Write
          //  1 = Read
          //  2 = Read & Write
          case 1:
            std::cout << "\nWrite Mode" << std::endl;
            uid = us.get_uid();
            index = us.get_index();
            us.access_file(0, index, uid);
            break;
          case 2:
            std::cout << "\nRead Mode" << std::endl;
            uid = us.get_uid();
            index = us.get_index();
            us.access_file(1, index, uid);
            break;
          case 3:
            std::cout << "\nRead & Write Mode" << std::endl;
            uid = us.get_uid();
            index = us.get_index();
            us.access_file(2, index, uid);
            break;
          case 4:
            us.get_current_user().view_account();
            break;
        }
      }
    } while (!exit);
  }
  catch (const exc::ClientException& e) {
    std::cout << e.what() << std::endl << std::endl;
  }
  return 0;
}
