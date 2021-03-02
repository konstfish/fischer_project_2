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
class pop3client_utils{
    private:

    public:
        pop3client_utils(){

        };

        int email_to_file(std::string write);
};