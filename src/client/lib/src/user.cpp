#include "../include/user.h"

User::User(){}

User::User(std::string name, std::string password, int age, std::string email, std::string type) {
    this->name = name;
    this->password = password;
    this->age = age;
    this->email = email;
    this->type = type;
}

std::string User::get_name() {
    return this->name;
}
std::string User::get_password() {
    return this->password;
}
int User::get_age() {
    return this->age;
}
std::string User::get_email() {
    return this->email;
}
std::string User::get_type() {
    return this->type;
}
void User::view_account() {
    // std::string enter;
    system("cls");
    std::cout << "\tACOUNT" << std::endl;
    std::cout << "Name: " << this->name << std::endl;
    std::cout << "Age: " << this->age << std::endl;
    std::cout << "Email: " << this->email;
    std::cin.get();
}


void User::set_name(std::string name) {
    this->name = name;
}