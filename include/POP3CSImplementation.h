/*
author: David Fischer
matnr:  i16079
file:   POP3CSImplementation.h
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

#include "ProtoInterface.h"

class POP3CSImplementation final : public pop3msg::POP3CS::Service {
    private:
        ProtoInterface &protoi;

    public:
        explicit POP3CSImplementation(ProtoInterface &pi):protoi(pi) {}

        grpc::Status get_mail_list(
            [[maybe_unused]] grpc::ServerContext* context, 
            const pop3msg::Operation* operation, 
            pop3msg::MailList* reply
        ) override {
            protoi.retrieve_messages(reply, operation->arg());

            return grpc::Status::OK;
        };

        grpc::Status save_mail(
            [[maybe_unused]] grpc::ServerContext* context, 
            const pop3msg::Operation* operation, 
            pop3msg::Success* reply
        ) override {
            protoi.save_mail(reply, operation->arg());

            return grpc::Status::OK;
        };

        grpc::Status delete_message(
            [[maybe_unused]] grpc::ServerContext* context, 
            const pop3msg::Operation* operation, 
            pop3msg::Success* reply
        ) override {
            protoi.delete_message(reply, operation->arg());

            return grpc::Status::OK;
        };

        grpc::Status disconnect(
            [[maybe_unused]] grpc::ServerContext* context, 
            [[maybe_unused]] const pop3msg::Operation* operation, 
            pop3msg::Success* reply
        ) override {
            protoi.disconnect(reply);

            return grpc::Status::OK;
        };
};