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

// helper function to write a string email to an .eml file in the build folder
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

// helper function to split strings using a certain identifier. returns a vector of strings
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

// helper function to split a pop3 email
vector<string> POP3client_utils::split_message(string res){
    vector<string> vec;

    size_t pos = 0;
    string token;

    string split = "\n";

    string from{""};
    string subject{""};
    string date{""};

    while ((pos = res.find(split)) != std::string::npos){
        token = res.substr(0, pos);
        
        if (token.find("From: ") != std::string::npos) {
            // From: - 
            token = token.substr(6, token.length());
            from = token;
        }

        if (token.find("Subject: ") != std::string::npos) {
            token = token.substr(9, token.length());
            subject = token;
        }

        if (token.find("Date: ") != std::string::npos) {
            token = token.substr(6, token.length());
            date = token;
        }

        res.erase(0, pos + split.length());
    }

    vec.push_back(from);
    vec.push_back(subject);
    vec.push_back(date);

    replace(vec[0].begin(), vec[0].end(), '\n', '\0');
    replace(vec[1].begin(), vec[1].end(), '\n', '\0');
    replace(vec[2].begin(), vec[2].end(), '\n', '\0');

    return vec;
}

// https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
// helper function to check if a message ends with a certain string
bool POP3client_utils::ends_with(string s, string suffix)
{
    return s.size() >= suffix.size() && s.rfind(suffix) == (s.size()-suffix.size());
}

// old function, not used, but still usefull
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
