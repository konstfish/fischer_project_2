/*
author: David Fischer
matnr:  i16079
file:   main.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include <iostream>
#include <fstream>

#include "cpp-subprocess/subprocess.hpp"
#include <gnutls/gnutls.h>

using namespace std;
using namespace subprocess;

// testdaten: user@nvs.com : 12345678

int main() {
    cout << "test" << endl;

    auto p = Popen({"gnutls-cli", "fortimail.konst.fish", "-p", "995"}, input{PIPE}, output{"test.txt"});
    auto msg = "USER user@nvs.com\n";

    p.send(msg, strlen(msg));




    /*auto res = p.communicate();

    std::cout << res.first.buf.data() << std::endl;

    p.wait();*/


}