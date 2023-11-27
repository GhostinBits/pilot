//
// Created by jiangqi on 2023-11-05.
//

#ifndef PILOT_NETWORKMANAGER_H
#define PILOT_NETWORKMANAGER_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <cstring>
#include <ctime>
#include <chrono>
#include <boost/process.hpp>
#include <vector>
#include <boost/regex.hpp>
#include <fstream>

class CLITools {
public:
    static std::string execute(const char*);
    static std::string execute_timeout(const char* command, int n);
    static std::string getWirelessInterfaces();
    static void updateWireless(const char*, const char*);
    static int authenticate(const char*, const char*);
    static std::vector<std::string> split(std::string, std::string);
};

#endif //PILOT_NETWORKMANAGER_H
