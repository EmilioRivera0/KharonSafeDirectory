#include "lib/include/usersystem.h"

int main () {
    UserSystem us = UserSystem();
    int option;
    bool isUser = false, exit = false;
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
            case 1:
                ;
                break;
            case 2:
                ;
                break;
            case 3:
                us.get_current_user().view_account();
                break;
            }
        }

    } while (!exit);

}