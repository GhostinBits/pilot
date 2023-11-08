cmake_minimum_required(VERSION 3.26)
project(pilot)

set(CMAKE_CXX_STANDARD 11)

#remove this line if not on macOS or change it to your boost directory
include_directories(/usr/local/Cellar/boost/1.83.0/include)

add_executable(pilot main.cpp
        CLITools.h
        CLITools.cpp)

target_link_libraries(pilot pthread)
