/*
author: David Fischer
matnr:  i16079
file:   util.h
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
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/basic_file_sink.h"

class pop3client_utils{
    private:

    public:
        pop3client_utils(){

        };

        int email_to_file(std::string write);

        std::vector<std::string> split(std::string res, std::string split);
        std::vector<std::string> split_message(std::string res);
        void print_messages(std::vector<std::vector<std::string>> messages);
};