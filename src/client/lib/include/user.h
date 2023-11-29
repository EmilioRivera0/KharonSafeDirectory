#include<iostream>
#include<string>

class User {
    private:
        std::string name;
        std::string password;
        int age;
        std::string email;
        std::string type;
    public:
        User();
        User(std::string, std::string, int, std::string, std::string);
        std::string get_name();
        std::string get_password();
        int get_age();
        std::string get_email();
        std::string get_type();
        void view_account();

        void set_name(std::string);
};