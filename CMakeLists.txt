cmake_minimum_required(VERSION 3.26)
project(pilot)

set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/Cellar/boost/1.83.0/include)

add_executable(pilot main.cpp
        CLITools.h
        CLITools.cpp
        NetworkInterface.cpp
        NetworkInterface.h)
