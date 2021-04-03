/*
author: David Fischer
matnr:  i16079
file:   Interactive.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "Interactive.h"

using namespace std;

int Interactive::run(){
    char input[100];
    string string_inp;
    string cmd;
    string arg;

    while (true){
        memset(input, 0, sizeof(input));
        cout << "bubble> ";
        cin.getline(input,sizeof(input));

        string_inp = input;
        parse_input(ref(cmd), ref(arg), string_inp);

        //cout << cmd << endl;
        //cout << arg << endl;

        int arg_int{0};

        //spdlog::get("console")->warn("Argument not valid");

        arg_int = parse_integer(arg);

        if(cmd == "dl" or cmd == "download"){
            if(arg_int == -1){
                spdlog::get("console")->warn("Argument not valid");
            }else{
                pop3msg::Success suc = client.save_mail(cmd, arg_int);
                if(suc.valid()){
                    spdlog::get("console")->info("Unable to save message with ID: {}", suc.message_id());
                }else{
                    spdlog::get("console")->info("Saved message with ID: {}", suc.message_id());
                }
            }
        }

        if(cmd == "rm" or cmd == "delete"){
            if(arg_int == -1){
                spdlog::get("console")->warn("Argument not valid");
            }else{
                pop3msg::Success suc = client.delete_message(cmd, arg_int);
                if(suc.valid()){
                    spdlog::get("console")->info("Unable to delete message with ID: {}", suc.message_id());
                }else{
                    spdlog::get("console")->info("Deleted message with ID: {}", suc.message_id());
                }
            }
        }

        if(cmd == "ls" or cmd == "list"){
            if(arg == ""){
                arg_int = 10;
            }
            if(arg_int == -1){
                spdlog::get("console")->warn("Argument not valid");
            }else{
                pop3msg::MailList ml = client.retrieve_messages(cmd, arg_int);
                cout << "test" << endl;
                print_messages(ml);
            }
        }

        if(cmd == "exit"){
            client.disconnect(cmd);
            return 1;
        }

        if(cmd == "help"){
            cout    << "Bubble Interactive Client:" << endl
                    << "dl / download <message_id>  - download email with id" << endl
                    << "rm / delete <message_id>    - delete email with id" << endl
                    << "ls / list <amount>          - list amount of mails" << endl
                    << "exit                        - disconnects the session" << endl
                    << "help                        - displays this message" << endl;
        }

    }

    return 0;
}

int Interactive::parse_integer(string arg){
    try{
        int arg_int = stoi(arg);
        return arg_int;
    } catch(std::exception& e){
        return -1;
    }
}

void Interactive::parse_input(string &c, string &a, string inp){
    c = "";
    a = "";

    size_t i{0};
    int mode{0};

    while(i < inp.size()){
        if(inp[i] == ' '){
            mode = 1;
        }else{
            if(mode == 0){
                c += inp[i];
            }else{
                a += inp[i];
            }
        }

        i += 1;
    }
}

void Interactive::print_messages(pop3msg::MailList ml){
    int size = ml.mails_size();
    int i = 0;

    tabulate::Table emails;

    emails.add_row({"Message ID", 
                        "Recieved From", 
                        "Subject",
                        "Date"});

    while(i < size){
        int mid = ml.mails(i).message_id();
        string message_id = to_string(mid);
        string from = ml.mails(i).from();
        string subject = ml.mails(i).subject();
        string date = ml.mails(i).date();

        emails.add_row({message_id, 
                        from, 
                        subject,
                        date});

        i += 1;
    }
    cout << emails << endl;

}