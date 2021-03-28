/*
author: David Fischer
matnr:  i16079
file:   Interactive.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once

#include <grpc/grpc.h>
#include <tabulate/table.hpp>

#include "POP3client.h"
#include "POP3CSClient.h"

#include "pop3.grpc.pb.h"

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