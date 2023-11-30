#include <semaphore.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include<iostream>
#include<map>
#include<vector>
#include <cstring>
#include <fstream>
#include "user.h"

class UserSystem {
    private:
        sem_t *cwait_sem{nullptr};
        int pipe_fd;
        const std::vector<std::string>& dir_entrys;
        std::string error_message;
        std::vector<User> users;
        User current_user;
    
    public:
        UserSystem(const std::vector<std::string>&);
        int principal_menu();
        bool sign_in();
        bool sign_up();
        bool verify_user(std::string, std::string, std::string);
        User get_current_user();
        User create_user(std::string, std::string, int, std::string, std::string);
        int loggedin_menu();
        void access_file(short, short, uid_t);
        short get_index();
        uid_t get_uid();
};
