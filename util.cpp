//
// Created by jiangqi on 2023-11-05.
//
#include "util.h"
namespace bp = boost::process;

std::string util::execute(const char* command) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string util::execute_timeout(const char* command, int n) {
    std::array<char, 128> buffer;
    std::string result;
    pid_t pid;
    const clock_t begin_time = clock();
    char command_buffer[1000];

    strcpy(command_buffer, command);
    strcat(command_buffer, " &> output.txt &");

    pid = fork();
    if(pid == 0) { // child process
        setpgid(getpid(), getpid());
        system(command_buffer);
    }
    else {   // parent process
        sleep(n);
        kill(-pid, SIGKILL);
        std::getline(std::ifstream("output.txt"), result, '\0');
        return result;
    }
}

std::string util::getWirelessInterfaces() {
    return util::execute("cat /proc/net/wireless");
}

void util::updateWireless(const char* ssid, const char* passphrase) {
    char commandHeader[100] = "sudo raspi-config nonint do_wifi_ssid_passphrase";
    util::execute(strcat(strcat(commandHeader, ssid), passphrase));
}
