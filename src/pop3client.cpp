/*
author: David Fischer
matnr:  i16079
file:   pop3client.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "pop3client.h"

using namespace std;

// http://www.hep.by/gnu/gnutls/Helper-functions-for-TCP-connections.html#Helper-functions-for-TCP-connections
int pop3client::socket_setup(){
    resolve();

    int err;

    sd = socket(addrfamily, SOCK_STREAM, 0);
    memset(&sa, '\0', sizeof(sa));

    sa.sin_family = addrfamily;
    // sa.sin_port = htons (atoi (PORT));
    sa.sin_port = htons(port);

    inet_pton(addrfamily, ipaddr.c_str(), &sa.sin_addr);
    err = connect(sd, (struct sockaddr *) &sa, sizeof(sa));


    if (err < 0)
    {
      fprintf (stderr, "Connect error\n");
      exit (1);
    }

    char buf[8193]{};

    recv(sd, buf, sizeof(buf) - 1, 0);

    string ret_recv = buf;
    cout << ret_recv << endl;

    return sd;
}

// https://stackoverflow.com/questions/52727565/client-in-c-use-gethostbyname-or-getaddrinfo
int pop3client::resolve() {
    hostent *rh     = gethostbyname(hostname.c_str());
    in_addr **ipptr = (struct in_addr **)rh->h_addr_list;

    addrfamily  = rh->h_addrtype;
    ipaddr      = inet_ntoa(*ipptr[0]);

    return 1;
}



void pop3client::socket_destruction(){
  shutdown (sd, SHUT_RDWR);
  close (sd);
}