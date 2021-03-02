/*
author: David Fischer
matnr:  i16079
file:   pop3client.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once

#include <iostream>

#include <stdio.h>
#include <unistd.h>


class pop3client{
    private:
        std::string end_signal = ".\r";

        std::string hostname;
        std::string port;
        bool tls;

    public:
        pop3client(std::string hn, std::string prt, bool tl) : hostname( hn ), port( prt ), tls( tl ){};

        void connect();
};