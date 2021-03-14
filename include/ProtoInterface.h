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
//#include "pop3.pb.h"
#include "pop3.grpc.pb.h"

class ProtoInterface{
    private:
    POP3client &client;

    public:
    ProtoInterface(POP3client &c):client( c ){};

    int retrieve_message_meta_proto(int message_id, pop3msg::MailMeta *temp_mail_meta);
    void retrieve_messages(pop3msg::MailList* ml, int amount);

    pop3msg::Success delete_message(int message_id);
    pop3msg::Success save_mail(int message_id);
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

        //reply->set_result(a * b);
        protoi.retrieve_messages(reply, operation->arg());

        std::cout << reply->mails_size() << std::endl;

        return grpc::Status::OK;
    } 
};

class POP3CSClient {
    private:
    private:
        std::unique_ptr<pop3msg::POP3CS::Stub> stub_;

    public:

    POP3CSClient(std::shared_ptr<grpc::Channel> channel) : stub_(pop3msg::POP3CS::NewStub(channel)) {};

    pop3msg::MailList retrieve_messages(std::string cmd, int arg){
        pop3msg::Operation op;

        op.set_cmd(cmd);
        op.set_arg(arg);

        pop3msg::MailList ml;

        grpc::ClientContext context;

        grpc::Status status = stub_->get_mail_list(&context, op, &ml);

        std::cout << ml.mails_size() << std::endl;

        return ml;
    }

};