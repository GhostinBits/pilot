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

class CLITools {
public:
    static std::string execute(const char *);
    static std::string getWirelessInterfaces();
};

#endif //PILOT_NETWORKMANAGER_H
