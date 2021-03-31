/*
author: David Fischer
matnr:  i16079
file:   Util.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <algorithm>

#include <spdlog/spdlog.h>

class POP3client_utils{
    public:
        POP3client_utils(){};

        int email_to_file(std::string write, std::string subject);

        std::vector<std::string> split(std::string res, std::string split);
        std::vector<std::string> split_message(std::string res);
        bool ends_with(std::string s, std::string suffix);
        // void print_messages(std::vector<std::vector<std::string>> messages);
};
