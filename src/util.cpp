/*
author: David Fischer
matnr:  i16079
file:   util.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "util.h"

using namespace std;

int pop3client_utils::email_to_file(string write){
    ofstream fstr;
    fstr.open ("temp.eml");
    fstr << write;
    fstr.close();
    return 0;
}

vector<string> pop3client_utils::split(string res, string split){
    vector<string> vec;

    size_t pos = 0;
    string token;

    while ((pos = res.find(split)) != std::string::npos){
        token = res.substr(0, pos);
        vec.push_back(token);
        res.erase(0, pos + split.length());
    }
    vec.push_back(res);

    int size = vec.size();
    replace(vec[size - 1].begin(), vec[size - 1].end(), '\n', '\0');

    return vec;
}

vector<string> pop3client_utils::split_message(string res){
    vector<string> vec;

    size_t pos = 0;
    string token;

    string split = "\n";

    while ((pos = res.find(split)) != std::string::npos){
        token = res.substr(0, pos);
        
        if (token.find("From: ") != std::string::npos) {
            // From: - 
            token = token.substr(6, token.length());
            vec.push_back(token);
        }

        if (token.find("Subject: ") != std::string::npos) {
            token = token.substr(9, token.length());
            vec.push_back(token);
        }

        if (token.find("Date: ") != std::string::npos) {
            token = token.substr(6, token.length());
            vec.push_back(token);
        }

        res.erase(0, pos + split.length());
    }

    replace(vec[0].begin(), vec[0].end(), '\n', '\0');
    replace(vec[1].begin(), vec[1].end(), '\n', '\0');
    replace(vec[2].begin(), vec[2].end(), '\n', '\0');

    return vec;
}

void pop3client_utils::print_messages(vector<vector<string>> messages){
    int size = messages.size();
    int i = 0;
    vector<string> temp;

    // TODO FIX OUTPUT

    while(i < size){
        temp = messages[i];
        string message_id = temp[3];
        string from = temp[0];
        string subject = temp[1];
        string date = temp[2];

        int size2 = temp.size();
        int i2 = 0;

        string concat = message_id;

        while(i2 < size2){
            cout << temp[i2] << endl;
            i2 += 1;
        }

        spdlog::get("console")->info("Message ID: {} - {}: {} ({})", message_id, from, subject, date);
        i += 1;
    }

}