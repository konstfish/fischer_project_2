/*
author: David Fischer
matnr:  i16079
file:   Interactive.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once

#include "POP3client.h"

class Interactive{
    private:
    POP3client &client;

    void parse_input(std::string &c, std::string &a, std::string inp);
    int parse_integer(std::string arg);

    public:
    Interactive(POP3client &c):client( c ){};

    int run();
};