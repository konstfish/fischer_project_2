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

    if(tls){
        gnutls_record_recv(gnutls_sd, buff, sizeof(buff) - 1);
    }else{
        recv(sd, buff, sizeof(buff) - 1, 0);
    }

    string rec = buff;
    cout << rec;
}

std::string pop3client::read_to_str(){
    char buff[8000]{};
    memset(buff, 0, sizeof(buff));

    if(tls){
        gnutls_record_recv(gnutls_sd, buff, sizeof(buff) - 1);
    }else{
        recv(sd, buff, sizeof(buff) - 1, 0);
    }

    string rec = buff;
    return rec;
}

// https://stackoverflow.com/questions/43264266/c-socket-send-and-connect
int pop3client::write(std::string msg){
    msg = msg + "\n";
    const char *cmsg = msg.c_str();
    int msg_len = strlen(cmsg);

    int result = 0;

    if(tls){
        result = gnutls_record_send(gnutls_sd, cmsg, msg_len);
    }else{
        result = send(sd, cmsg, msg_len, 0);
    }

    return result;
}

int pop3client::temp(){
    resolve();
    socket_setup();
    
    if(tls){
        gnutls_setup();
    }

    // temporärer pfusch
    usleep(100000);

    read();
    write("USER user@nvs.com");
    read();
    write("PASS 12345678");
    read();
    write("RETR 1");
    read();

    write("RETR 1");
    utility.email_to_file(read_to_str());

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


// http://www.hep.by/gnu/gnutls/Simple-client-example-with-X_002e509-certificate-support.html#Simple-client-example-with-X_002e509-certificate-support
int pop3client::gnutls_setup(){
    int ret;
    const char *err;

    gnutls_global_init ();
    gnutls_certificate_allocate_credentials (&xcred);

    gnutls_certificate_set_x509_system_trust(xcred);
    gnutls_init (&gnutls_sd, GNUTLS_CLIENT);

    //gnutls_session_set_ptr (session, (void *) hostname);
    //gnutls_transport_set_ptr (gnutls_sd, (gnutls_transport_ptr_t) sd);
    // wird ned so wichtig sein, geht auch ohne

    gnutls_server_name_set (gnutls_sd, GNUTLS_NAME_DNS, hostname.c_str(), strlen(hostname.c_str()));
    ret = gnutls_priority_set_direct (gnutls_sd, "NORMAL", &err);
    gnutls_credentials_set (gnutls_sd, GNUTLS_CRD_CERTIFICATE, xcred);

    gnutls_transport_set_int(gnutls_sd, sd);
    gnutls_handshake_set_timeout(gnutls_sd, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);

    int err_handshake = gnutls_handshake(gnutls_sd);
    if(err_handshake){
        return 1;
    }

    return 0;
}

void pop3client::gnutls_destruction(){
    gnutls_bye (gnutls_sd, GNUTLS_SHUT_RDWR);

    gnutls_deinit (gnutls_sd);
    gnutls_certificate_free_credentials (xcred);
    gnutls_global_deinit ();
}


void pop3client::socket_destruction(){
  shutdown (sd, SHUT_RDWR);
  close (sd);
}