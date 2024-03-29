/*
author: David Fischer
matnr:  i16079
file:   POP3client.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once

#include "Util.h"

#include <gnutls/gnutls.h>
#include <spdlog/spdlog.h>

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
#include <vector> 

// POP3client class, interacts with the POP3 Server
class POP3client{
    private:
        std::string hostname;
        u_int16_t port;

        std::string ipaddr;
        int addrfamily;

        struct sockaddr_in sa;
        int sd;
        
        bool socket_established{false};

        bool tls;

        gnutls_certificate_credentials_t xcred;
        gnutls_session_t gnutls_sd;

        POP3client_utils utility;

        bool tls_established{false};

        int socket_setup();
        int resolve();

        int gnutls_setup();

        void gnutls_destruction();
        void socket_destruction();

    public:
        POP3client(std::string hn, u_int16_t prt, bool tl) : hostname( hn ), port( prt ), tls( tl ){
            spdlog::get("logger")->info("Connecting to {} on Port {}. TLS Enabled ({})", hn, prt, tl);
        };

        ~POP3client(){
            if(socket_established){
                socket_destruction();
            }
            if(tls_established){
                gnutls_destruction();
            }
        };

        int establish_connection();
        int login(std::string user, std::string password);
        int quit();

        void read();
        std::string read_to_str();
        std::string read_to_end();
        int write(std::string msg);

        int get_total_messages();
        int delete_message(int message_id);
        
        std::vector<std::string> retrieve_message_metadata(int message_id);
        std::vector<std::vector<std::string>> retrieve_messages(int amount);

        int save_mail(int message_id);
};