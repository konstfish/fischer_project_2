/*
author: David Fischer
matnr:  i16079
file:   Protointerface.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "POP3CSClient.h"

using namespace std;

pop3msg::MailList POP3CSClient::retrieve_messages(std::string cmd, int arg){
    pop3msg::Operation op;

    op.set_cmd(cmd);
    op.set_arg(arg);

    pop3msg::MailList ml;

    grpc::ClientContext context;

    grpc::Status status = stub_->get_mail_list(&context, op, &ml);

    return ml;
}

pop3msg::Success POP3CSClient::delete_message(std::string cmd, int arg){
    pop3msg::Operation op;

    op.set_cmd(cmd);
    op.set_arg(arg);

    pop3msg::Success suc;

    grpc::ClientContext context;

    grpc::Status status = stub_->delete_message(&context, op, &suc);

    return suc;
}

pop3msg::Success POP3CSClient::save_mail(std::string cmd, int arg){
    pop3msg::Operation op;

    op.set_cmd(cmd);
    op.set_arg(arg);

    pop3msg::Success suc;

    grpc::ClientContext context;

    grpc::Status status = stub_->save_mail(&context, op, &suc);

    return suc;
}

pop3msg::Success POP3CSClient::disconnect(std::string cmd){
    pop3msg::Operation op;

    op.set_cmd(cmd);

    pop3msg::Success suc;

    grpc::ClientContext context;

    grpc::Status status = stub_->disconnect(&context, op, &suc);

    return suc;
}