/*
author: David Fischer
matnr:  i16079
file:   main.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include <iostream>

//#include <asio/ssl.hpp>
#include <asio.hpp>
#include "pop3client.h"

using namespace asio::ip;
using namespace std;
#include<string.h>

// testdaten: user@nvs.com : 12345678

int main() {
    pop3client c("fortimail.konst.fish", "110", false);
    c._connect();
    c._quit();
  
  
  /*  string port{"110"};
    string port_tls{"995"};
    string host{"fortimail.konst.fish"};

    string end_signal = ".\r";

    //tcp::iostream strm{"10.0.1.5", port};

    asio::io_context ctx; 
    tcp::resolver resolver{ctx};
    //asio::ssl::context ssl_ctx(asio::ssl::context::tlsv12);

    try {
        auto results = resolver.resolve(host, port);
        tcp::socket sock{ctx}; // IO object

        //asio::ssl::stream<asio::ip::tcp::socket> sock(ctx, ssl_ctx);


        asio::connect(sock, results); //→ function! cout << ”connected” << endl;
        tcp::iostream strm{std::move(sock)};

        if(strm) {
        try {
            string data;
            getline(strm, data);
            cout << data << endl;
            
            strm << "USER user@nvs.com" << endl;

            getline(strm, data);
            cout << data << endl;

            strm << "PASS 12345678" << endl;

            getline(strm, data);
            cout << data << endl;

            strm << "NOOP" << endl;

            getline(strm, data);
            cout << data << endl;

            strm << "LIST" << endl;

            while ( data != end_signal ){
                getline(strm, data);
                cout << data << endl;
            }

            data = "";

            strm << "RETR 1" << endl;

            while ( data != end_signal ){
                getline(strm, data);
                cout << data << endl;
            }

            strm << "NOOP" << endl;

            getline(strm, data);
            cout << data << endl;

            strm << "QUIT" << endl;

            getline(strm, data);
            cout << data << endl;
            
            strm.close();


        } catch(exception e) {
            cout << "err" << endl;
        }
    } else { 
        cout << "could not connect" << endl;
    }

    } catch(...){
        cout << "test" << endl;
    }
*/
    

    return 0;
}

/*
OUTPUT

+OK mail service ready.
+OK
+OK Logged in.
+OK
+OK 1 messages:
1 753
.
+OK 753 octets
Return-Path: <david@konst.fish>
Received: from [10.0.0.100] (82-149-123-143.wco.wellcom.at [82.149.123.143])
	(user=david@konst.fish mech=DIGEST-MD5 bits=0)
	by mail.konst.fish  with ESMTP id 11BCON0q024013-11BCON0r024013
	(version=TLSv1.2 cipher=DHE-RSA-AES256-SHA256 bits=256 verify=NO)
	for <user@nvs.com>; Thu, 11 Feb 2021 13:24:24 +0100
From: David Fischer <david@konst.fish>
Content-Type: text/plain;
	charset=us-ascii
Content-Transfer-Encoding: 7bit
Mime-Version: 1.0 (Mac OS X Mail 14.0 \(3654.40.0.2.32\))
Subject: Test Mail
Message-Id: <265272AB-7CE2-4414-9A84-7ADE1A2BABA8@konst.fish>
Date: Thu, 11 Feb 2021 13:24:22 +0100
To: user@nvs.com
X-Mailer: Apple Mail (2.3654.40.0.2.32)
X-FEAS-AUTH-USER: david@konst.fish

Hallo!
.
+OK
+OK Logging out.

*/