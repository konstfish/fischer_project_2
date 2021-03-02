/*
author: David Fischer
matnr:  i16079
file:   main.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "pop3client.h"

using namespace std;

int main() {
    //pop3client c("fortimail.konst.fish", 110, false);
    pop3client c("fortimail.konst.fish", 995, true);
    c.temp();
}