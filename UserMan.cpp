//
// Created by jiangqi on 2023-11-28.
//

#include "UserMan.h"
#include <iostream>

UserMan::UserMan(Session* s) {
    session = s;
    loadUser();
}

int UserMan::loadUser() {
    std::ifstream f("users.json");
    userData = json::parse(f);
    session -> loadUser();
}

int UserMan::addUser(std::string username, std::string password) {

    std::cout << userData;
    userData["users"][userData["users"].size()]["username"] = username;
    userData["users"][userData["users"].size() - 1]["password"] = hash(password);
    std::cout << userData;
    // dump into json file
    std::ofstream o("users.json");
    o << std::setw(2) << userData << std::endl;
    return 1;
}

int UserMan::deleteUser(std::string username) {
    json newData;
    for (int i = 0; i < userData["users"].size(); i++) {
        if (userData["users"][i]["username"] != username) {
            newData["users"] += userData["users"][i];
        }
    }
    std::ofstream o("users.json");
    userData = newData;
    o << std::setw(2) << newData << std::endl;
    return 1;
}

std::string UserMan::getUsers() {
    /*
     * return a table of users that will be rendered in the user management page
     */
    std::string result;

    loadUser();

    result = R"(
<table class="table">
  <tbody>
)";

    for (int i = 0; i < userData["users"].size(); i++) {
        //userData["users"][i]["username"]
        result += R"(
    <tr>
      <th scope="row">
)";
        result += std::to_string(i + 1);
        result += R"(</th><td>
        )";
        result += userData["users"][i]["username"];
        if (session -> getCurrentUser() != userData["users"][i]["username"]) {
            result += R"(
<td>

<a href="
        )";
            result += R"(/deleteuser?username=)";
            result += userData["users"][i]["username"];
            result += R"(
" class="btn btn-danger">
                Delete
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-trash" viewBox="0 0 16 16">
  <path d="M5.5 5.5A.5.5 0 0 1 6 6v6a.5.5 0 0 1-1 0V6a.5.5 0 0 1 .5-.5m2.5 0a.5.5 0 0 1 .5.5v6a.5.5 0 0 1-1 0V6a.5.5 0 0 1 .5-.5m3 .5a.5.5 0 0 0-1 0v6a.5.5 0 0 0 1 0z"></path>
  <path d="M14.5 3a1 1 0 0 1-1 1H13v9a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2V4h-.5a1 1 0 0 1-1-1V2a1 1 0 0 1 1-1H6a1 1 0 0 1 1-1h2a1 1 0 0 1 1 1h3.5a1 1 0 0 1 1 1zM4.118 4 4 4.059V13a1 1 0 0 0 1 1h6a1 1 0 0 0 1-1V4.059L11.882 4zM2.5 3h11V2h-11z"></path>
</svg>
              </a>
      </td>
    </tr>
            )";
        }
        else {
            result += R"(
<td>

<a class="btn btn-outline-danger">
                Not unavailable for logged in user
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-x-lg" viewBox="0 0 16 16">
  <path d="M2.146 2.854a.5.5 0 1 1 .708-.708L8 7.293l5.146-5.147a.5.5 0 0 1 .708.708L8.707 8l5.147 5.146a.5.5 0 0 1-.708.708L8 8.707l-5.146 5.147a.5.5 0 0 1-.708-.708L7.293 8z"></path>
</svg>
              </a>
      </td>
    </tr>
            )";
        }

    }

    result += R"(
  </tbody>
</table>
)";

    return result;
}
