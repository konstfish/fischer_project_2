/*
author: David Fischer
matnr:  i16079
file:   Protointerface.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once

#include <grpcpp/grpcpp.h>

#include "POP3client.h"
#include "pop3.grpc.pb.h"

class ProtoInterface{
    private:
        POP3client &client;

    public:
        ProtoInterface(POP3client &c):client( c ){};

        int retrieve_message_meta_proto(int message_id, pop3msg::MailMeta *temp_mail_meta);
        int retrieve_messages(pop3msg::MailList *ml, int amount);

        int delete_message(pop3msg::Success *suc, int message_id);
        int save_mail(pop3msg::Success *suc, int message_id);

        int disconnect(pop3msg::Success *suc);
};

class POP3CSImplementation final : public pop3msg::POP3CS::Service {
    private:
        ProtoInterface &protoi;

    public:
        explicit POP3CSImplementation(ProtoInterface &pi):protoi(pi) {}

        grpc::Status get_mail_list(
            grpc::ServerContext* context, 
            const pop3msg::Operation* operation, 
            pop3msg::MailList* reply
        ) override {
            protoi.retrieve_messages(reply, operation->arg());

            return grpc::Status::OK;
        };

        grpc::Status save_mail(
            grpc::ServerContext* context, 
            const pop3msg::Operation* operation, 
            pop3msg::Success* reply
        ) override {
            protoi.save_mail(reply, operation->arg());

            return grpc::Status::OK;
        };

        grpc::Status delete_message(
            grpc::ServerContext* context, 
            const pop3msg::Operation* operation, 
            pop3msg::Success* reply
        ) override {
            protoi.delete_message(reply, operation->arg());

            return grpc::Status::OK;
        };

        grpc::Status disconnect(
            grpc::ServerContext* context, 
            const pop3msg::Operation* operation, 
            pop3msg::Success* reply
        ) override {
            protoi.disconnect(reply);

            return grpc::Status::OK;
        };
};

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