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
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include <gnutls/gnutls.h>
#include <gnutls/gnutlsxx.h>
#include <gnutls/x509.h>

class pop3client{
    private:
        std::string end_signal = ".\r";

        std::string hostname;
        u_int16_t port;

        std::string ipaddr;
        int addrfamily;

        struct sockaddr_in sa;
        int sd;

        bool tls;
    public:
        pop3client(std::string hn, u_int16_t prt, bool tl) : hostname( hn ), port( prt ), tls( tl ){};

        ~pop3client(){
            socket_destruction();
        };

        int temp();

        int socket_setup();
        int resolve();

        void read();
        int write(std::string msg);

        int gnutls_setup();

        void gnutls_destruction();
        void socket_destruction();
};