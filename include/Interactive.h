/*
author: David Fischer
matnr:  i16079
file:   Interactive.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once


#include "POP3CSClient.h"

#include <spdlog/spdlog.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <tabulate.hpp>
//#include <table.hpp>
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <grpcpp/grpcpp.h>

#include "pop3.grpc.pb.h"
#pragma GCC diagnostic pop

// Class Interactive, Enables an Interactive Shell to dynamically comunicate with the 
// POP3 Server in a bash-style manner
class Interactive{
    private:
        POP3CSClient &client;

        void parse_input(std::string &c, std::string &a, std::string inp);
        int parse_integer(std::string arg);
        void print_messages(pop3msg::MailList ml);

    public:
        Interactive(POP3CSClient &c):client( c ){};

        int run();
};
