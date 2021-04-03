/*
author: David Fischer
matnr:  i16079
file:   POP3CSClient.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <grpcpp/grpcpp.h>

#include "pop3.grpc.pb.h"
#pragma GCC diagnostic pop

class POP3CSClient {
    private:
        std::unique_ptr<pop3msg::POP3CS::Stub> stub_;

    public:
        POP3CSClient(std::shared_ptr<grpc::Channel> channel) : stub_(pop3msg::POP3CS::NewStub(channel)) {};

        pop3msg::MailList retrieve_messages(std::string cmd, int arg);

        pop3msg::Success delete_message(std::string cmd, int arg);

        pop3msg::Success save_mail(std::string cmd, int arg);

        pop3msg::Success disconnect(std::string cmd);
};