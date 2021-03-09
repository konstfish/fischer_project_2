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
#include "pop3.pb.h"

class ProtoInterface{
    private:
    POP3client &client;

    public:
    ProtoInterface(POP3client &c):client( c ){

    };

    int retrieve_message_meta_proto(int message_id, pop3msg::MailMeta *temp_mail_meta);
    pop3msg::MailList retrieve_messages(int amount);

    pop3msg::Success delete_message(int message_id);
    pop3msg::Success save_mail(int message_id);
};