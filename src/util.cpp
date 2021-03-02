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

int pop3client_utils::email_to_file(std::string write){
    ofstream fstr;
    fstr.open ("temp.eml");
    fstr << write;
    fstr.close();
    return 0;
}