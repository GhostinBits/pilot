//
// Created by jiangqi on 2023-11-28.
//

#include "Session.h"
#include <iostream>

Session::Session() {
    loadUser();
}

int Session::loginStatus() {
    return isLoggedIn;
}

int Session::loadUser() {
    std::ifstream f("users.json");
    userData = json::parse(f);
}

int Session::authenticate(std::string username, std::string password) {

    for (int i = 0; i < userData["users"].size(); i++) {
        if (userData["users"][i]["username"] == username) {
            if (userData["users"][i]["password"] == hash(password)) {
                isLoggedIn = 1;
                currentUser = username;
                return isLoggedIn;
            }
        }
    }

    isLoggedIn = 0;
    return isLoggedIn;
}

std::string Session::getCurrentUser() {
    return currentUser;
}
