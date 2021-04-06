/*
author: David Fischer
matnr:  i16079
file:   Protointerface.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once

#include "POP3client.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <grpcpp/grpcpp.h>

#include "pop3.grpc.pb.h"
#pragma GCC diagnostic pop

// ProtoInterface Class, converts POP3Client vectors to Proto Constructs
class ProtoInterface{
    private:
        POP3client &client;

    public:
        ProtoInterface(POP3client &c):client( c ){};

        int retrieve_message_meta_proto(int message_id, pop3msg::MailMeta *temp_mail_meta);
        int retrieve_message_meta_proto_dry(int message_id);
        
        int retrieve_messages(pop3msg::MailList *ml, int amount);

        int delete_message(pop3msg::Success *suc, int message_id);
        int save_mail(pop3msg::Success *suc, int message_id);

        int disconnect(pop3msg::Success *suc);
};
