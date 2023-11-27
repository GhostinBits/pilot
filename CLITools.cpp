//
// Created by jiangqi on 2023-11-05.
//
#include "CLITools.h"
namespace bp = boost::process;

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

std::string CLITools::execute_timeout(const char* command, int n) {
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

std::string CLITools::getWirelessInterfaces() {
    return CLITools::execute("cat /proc/net/wireless");
}

void CLITools::updateWireless(const char* ssid, const char* passphrase) {
    char commandHeader[100] = "sudo raspi-config nonint do_wifi_ssid_passphrase";
    CLITools::execute(strcat(strcat(commandHeader, ssid), passphrase));
}

std::vector<std::string> CLITools::split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr (pos_start));
    return res;
}

int CLITools::authenticate(const char* username, const char* passwd) {

    // debugging only
    if (strcmp(username, "debug") == 0 && strcmp(passwd, "debug") == 0) {
        return 1;
    }

    char hashCmdHeader[100] = "cat /etc/shadow | grep ";
    char pwdChkCmd[200] = "python -c 'import crypt, os; print(crypt.crypt(";
    std::string pattern;
    std::string salt;
    std::string passwordHash;
    std::string calculatedHash;
    std::vector<std::string> subs;
    static const boost::regex e(".*&");
    boost::match_results<std::string::const_iterator> results;
    pattern = CLITools::execute(strcat(hashCmdHeader, username));

    if (boost::regex_match(pattern, results, e)) {
        salt = results[0];
    }
    else {
        return 0;
    }

    subs = split(pattern, "&");
    passwordHash = subs.back();

    //python -c 'import crypt, os; print(crypt.crypt(     a, b))'

    calculatedHash = CLITools::execute(strcat(strcat(strcat(strcat(pwdChkCmd, passwd), ", "), salt.c_str()), "))'"));
    if (passwordHash.find(calculatedHash) != std::string::npos) {
        return 1;
    }
    else return 0;

}
