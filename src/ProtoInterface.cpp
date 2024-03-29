/*
author: David Fischer
matnr:  i16079
file:   ProtoInterface.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "ProtoInterface.h"

using namespace std;

// parses the vector of strings and converts it to a MailMeta Proto Construct
int ProtoInterface::retrieve_message_meta_proto(int message_id, pop3msg::MailMeta *temp_mail_meta){
    vector<string> res_vec = client.retrieve_message_metadata(message_id);

    temp_mail_meta->set_from(res_vec[0]);
    temp_mail_meta->set_subject(res_vec[1]);
    temp_mail_meta->set_date(res_vec[2]);
    temp_mail_meta->set_message_id(message_id);

    if(res_vec[0] == "MARKED FOR DELETION"){
        return 1;
    }

    return 0;
}

// dry run helper function
int ProtoInterface::retrieve_message_meta_proto_dry(int message_id){
    vector<string> res_vec = client.retrieve_message_metadata(message_id);

    if(res_vec[0] == "MARKED FOR DELETION"){
        return 1;
    }

    return 0;
}

// parses fills a MailList Proto Construct with MailMeta Proto Consturuct
int ProtoInterface::retrieve_messages(pop3msg::MailList *temp_mail_list, int amount){
    int total_messages = client.get_total_messages();

    if(amount > total_messages){
        amount = total_messages;
    }

    int retrieve{0};
    int check{0};

    // dry run
    int cur_message = total_messages;

    while(cur_message > (total_messages - amount)){
        check = retrieve_message_meta_proto_dry(cur_message);

        if(check){
            retrieve += 1;
        }

        cur_message -= 1;
    }

    // retrieve (overcompensate for deleted messages)
    int i = 1;
    while(i <= retrieve){
        retrieve_message_meta_proto((total_messages + i), temp_mail_list->add_mails());

        i += 1;
    }

    // run
    cur_message = total_messages;

    while(cur_message > (total_messages - amount)){
        check = retrieve_message_meta_proto(cur_message, temp_mail_list->add_mails());

        cur_message -= 1;
    }

    return 0;
}

// instructs the client to save an email and creates a Success Proto Construct
int ProtoInterface::save_mail(pop3msg::Success *suc, int message_id){
    int check = client.save_mail(message_id);

    suc->set_message_id(message_id);
    suc->set_valid(check);

    return check;
}

// instructs the client to delete an email and creates a Success Proto Construct
int ProtoInterface::delete_message(pop3msg::Success *suc, int message_id){
    int check = client.delete_message(message_id);

    suc->set_message_id(message_id);
    suc->set_valid(check);

    return check;
}

// instructs the client to disconnect from the POP3 Server and creates a Sucess Proto Construct
int ProtoInterface::disconnect(pop3msg::Success *suc){
    int check = client.quit();

    suc->set_valid(check);

    return check;
}
