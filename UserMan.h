//
// Created by jiangqi on 2023-11-28.
//

#ifndef PILOT_USERMAN_H
#define PILOT_USERMAN_H

#include <string>
#include <fstream>
#include <functional>
#include "nlohmann/json.hpp"
#include "Session.h"

using json = nlohmann::json;

class UserMan {
private:
    std::hash<std::string> hash;
    std::string username;
    std::string passwordHash;  // hashed, only exception is debug:debug
    json userData;
    Session* session;

    int loadUser();  // load usernames and passwords from a json file

public:
    explicit UserMan(Session*);
    int deleteUser(std::string);
    int addUser(std::string, std::string);
    std::string getUsers();
};


#endif //PILOT_USERMAN_H
