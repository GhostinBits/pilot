//
// Created by jiangqi on 2023-11-28.
//

#ifndef PILOT_SESSION_H
#define PILOT_SESSION_H

#include <string>
#include <fstream>
#include <functional>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Session {
private:
    std::hash<std::string> hash;
    std::string currentUser;
    json userData;
    int isLoggedIn; // 0 for false, 1 for true



public:
    Session();
    int loadUser();  // load usernames and passwords from a json file
    int authenticate(std::string, std::string);
    std::string getCurrentUser();
    int loginStatus();
};


#endif //PILOT_SESSION_H
