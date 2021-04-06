/*
author: David Fischer
matnr:  i16079
file:   POP3CSClient.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "POP3CSClient.h"

using namespace std;

// grpc client function to retrieve a MailList from the Server
pop3msg::MailList POP3CSClient::retrieve_messages(std::string cmd, int arg){
    pop3msg::Operation op;

    op.set_cmd(cmd);
    op.set_arg(arg);

    pop3msg::MailList ml;

    grpc::ClientContext context;

    grpc::Status status = stub_->get_mail_list(&context, op, &ml);

    return ml;
}

// grpc client function, which instructs the Server to delete an email
pop3msg::Success POP3CSClient::delete_message(std::string cmd, int arg){
    pop3msg::Operation op;

    op.set_cmd(cmd);
    op.set_arg(arg);

    pop3msg::Success suc;

    grpc::ClientContext context;

    grpc::Status status = stub_->delete_message(&context, op, &suc);

    return suc;
}

// grpc client function, which instructs the Server to save a mail locally
pop3msg::Success POP3CSClient::save_mail(std::string cmd, int arg){
    pop3msg::Operation op;

    op.set_cmd(cmd);
    op.set_arg(arg);

    pop3msg::Success suc;

    grpc::ClientContext context;

    grpc::Status status = stub_->save_mail(&context, op, &suc);

    return suc;
}

// grpc client function, which instructs the Server to disconnect from the POP3 Server
pop3msg::Success POP3CSClient::disconnect(std::string cmd){
    pop3msg::Operation op;

    op.set_cmd(cmd);

    pop3msg::Success suc;

    grpc::ClientContext context;

    grpc::Status status = stub_->disconnect(&context, op, &suc);

    return suc;
}