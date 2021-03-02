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

    return sd;
}

// https://stackoverflow.com/questions/43264266/c-socket-send-and-connect
void pop3client::read(){
    char buff[8000]{};
    memset(buff, 0, sizeof(buff));

    recv(sd, buff, sizeof(buff) - 1, 0);
    string rec = buff;
    cout << rec;
}

// https://stackoverflow.com/questions/43264266/c-socket-send-and-connect
int pop3client::write(std::string msg){
    msg = msg + "\n";
    const char *cmsg = msg.c_str();
    int msg_len = strlen(cmsg);

    int result = send(sd, cmsg, msg_len, 0);

    return result;
}

int pop3client::temp(){
    resolve();
    socket_setup();

    // temporärer pfusch
    usleep(100000);

    read();
    write("USER user@nvs.com");
    read();
    write("PASS 12345678");
    read();
    write("RETR 1");
    read();

    return 0;
}

// https://stackoverflow.com/questions/52727565/client-in-c-use-gethostbyname-or-getaddrinfo
int pop3client::resolve() {
    hostent *rh     = gethostbyname(hostname.c_str());
    in_addr **ipptr = (struct in_addr**)rh->h_addr_list;

    addrfamily  = rh->h_addrtype;
    ipaddr      = inet_ntoa(*ipptr[0]);

    return 1;
}

/*

// http://www.hep.by/gnu/gnutls/Simple-client-example-with-X_002e509-certificate-support.html#Simple-client-example-with-X_002e509-certificate-support
int gnutls_setup(){
    gnutls_global_init ();
    gnutls_certificate_allocate_credentials (&xcred);

}*/


void pop3client::socket_destruction(){
  shutdown (sd, SHUT_RDWR);
  close (sd);
}