/*
author: David Fischer
matnr:  i16079
file:   Util.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "Util.h"

using namespace std;

int POP3client_utils::email_to_file(string write, string subject){
    ofstream fstr;

    // quick & dirty fix for question mark at the end of subject
    subject = subject.substr(0, subject.size()-1);
    string path = subject + ".eml";
    fstr.open (path);

    // remove dot from end of email
    write = write.substr(0, write.size()-3);

    fstr << write;

    fstr.close();

    return 0;
}

vector<string> POP3client_utils::split(string res, string split){
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

vector<string> POP3client_utils::split_message(string res){
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

// https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
bool POP3client_utils::ends_with(string s, string suffix)
{
    return s.size() >= suffix.size() && s.rfind(suffix) == (s.size()-suffix.size());
}

/*
void POP3client_utils::print_messages(vector<vector<string>> messages){
    int size = messages.size();
    int i = 0;
    vector<string> temp;

    tabulate::Table emails;

    emails.add_row({"Message ID", 
                        "Recieved From", 
                        "Subject",
                        "Date"});
    while(i < size){
        temp = messages[i];
        string message_id = temp[3];
        string from = temp[0];
        string subject = temp[1];
        string date = temp[2];

        emails.add_row({message_id, 
                        from, 
                        subject,
                        date});

        i += 1;
    }
    cout << emails << endl;

}*/