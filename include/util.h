/*
author: David Fischer
matnr:  i16079
file:   util.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once

#include <iostream>

class pop3client_utils{
    private:
        int temp;
    public:
        pop3client_utils();

        void email_to_file();
};