//
// Created by jiangqi on 2023-11-05.
//
#include "CLITools.h"

std::string CLITools::execute(const char* command) {
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

std::string CLITools::getWirelessInterfaces() {
    return CLITools::execute("cat /proc/net/wireless");
}

void CLITools::updateWireless(const char* ssid, const char* passphrase) {
    CLITools::execute(strcat(strcat(
            (char*)"sudo raspi-config nonint do_wifi_ssid_passphrase", ssid), passphrase));
}
