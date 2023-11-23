// necessary libraries -------->
#include "lib/include/control_dir.h"

// server execution starting point -------->
int main (int argc, char **argv) {
  // main function variables
  std::vector<struct file_path*> dir_entrys{};
  try {
    // check if command ir executed as a root user
    if (getuid() != ROOT_UID) 
      throw exc::ServerException("\nExecute server as root user.\n");
    // check if command line arguments are specified
    if (argc == 1)
      throw exc::ServerException("\nIndicate an objective directory as the first command line parameter.\n");
    // take control of the specified directory and its entrys
    control_directory(*(argv+1), dir_entrys);
  }
  catch (const exc::ServerException& e) {
    std::cout << e.what() << std::endl << std::endl;
    PROGRAM_ERROR
  }
  return 0;
}
