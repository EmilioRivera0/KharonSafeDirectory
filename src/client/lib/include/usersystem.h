#include<iostream>
#include<map>
#include<vector>
#include<string>
#include <cstring>
#include "user.h"

class UserSystem {
    private:
        std::string error_message;
        std::vector<User> users;
        User current_user;
    
    public:
        UserSystem();
        int principal_menu();
        bool sign_in();
        bool sign_up();
        bool verify_user(std::string, std::string, std::string);
        User get_current_user();
        User create_user(std::string, std::string, int, std::string, std::string);

        int loggedin_menu();
};