#include "../include/usersystem.h"

//Functions ---------->
UserSystem::UserSystem(){}

int UserSystem::principal_menu() {
    system("cls");
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
        system("cls");
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
    system("cls");
    int option, age;
    std::string name, password, email;
    do {
        system("cls");
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
    system("cls");
    int option;
    std::cout << "\t" << this->current_user.get_name() << std::endl;
    std::cout << "1. Write file" << std::endl;
    std::cout << "2. Read file" << std::endl;
    std::cout << "3. View account" << std::endl;
    std::cout << "0. Sign out" << std::endl;
    std::cout << "Option: ";
    std::cin >> option;
    std::cin.get();
    return option;
}