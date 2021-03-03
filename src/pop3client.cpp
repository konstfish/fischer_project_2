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

int pop3client::establish_connection(){
    int check{0};

    check = resolve();
    if(check){
        socket_established = false;
        spdlog::get("console")->error("Unable to Resolve Hostname, exiting");
        spdlog::get("logger")->error("Unable to Resolve Hostname, exiting");
        return 1;
    }

    check = socket_setup();
    if(check){
        spdlog::get("console")->error("Unable to establish Socket Connection, exiting");
        spdlog::get("logger")->error("Unable to Resolve Hostname, exiting");
        return 1;
    }

    if(tls){
        check = gnutls_setup();
        if(check){
            spdlog::get("console")->error("Unable to establish TLS Handshake, exiting");
            spdlog::get("logger")->error("Unable to Resolve Hostname, exiting");
            return 1;
        }
    }

    spdlog::get("console")->info("Connection successfully established");
    spdlog::get("logger")->info("Connection successfully established");

    return 0;
}

void pop3client::debug(){
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
}

// https://stackoverflow.com/questions/52727565/client-in-c-use-gethostbyname-or-getaddrinfo
// https://stackoverflow.com/questions/38002016/problems-with-gethostbyname-c
int pop3client::resolve() {
    hostent *rh     = gethostbyname(hostname.c_str());
    if(rh == NULL){
        return 1;
    }

    in_addr **ipptr = (struct in_addr**)rh->h_addr_list;

    addrfamily  = rh->h_addrtype;
    ipaddr      = inet_ntoa(*ipptr[0]);

    return 0;
}

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
      return 1;
    }

    socket_established = true;
    return 0;
}

// http://www.hep.by/gnu/gnutls/Simple-client-example-with-X_002e509-certificate-support.html#Simple-client-example-with-X_002e509-certificate-support
int pop3client::gnutls_setup(){
    int ret;
    const char *err;

    // X.509 authentication is used
    gnutls_global_init();
    gnutls_certificate_allocate_credentials(&xcred);
    gnutls_certificate_set_x509_system_trust(xcred);

    // gnutls initialization (gnutls_sd = gnutls session descriptor)
    gnutls_init (&gnutls_sd, GNUTLS_CLIENT);

    // verify server certificate
    gnutls_server_name_set (gnutls_sd, GNUTLS_NAME_DNS, hostname.c_str(), strlen(hostname.c_str()));
    
    // gnutls priorities (priority strings specify the TLS sessions handshake algorithms and options in a compact, easy-to-use format)
    // https://gnutls.org/manual/html_node/Priority-Strings.html
    ret = gnutls_priority_set_direct (gnutls_sd, "NORMAL", &err);

    // apply credentials to current session
    gnutls_credentials_set (gnutls_sd, GNUTLS_CRD_CERTIFICATE, xcred);

    // link socket descriptor to gnutls socket descriptor
    gnutls_transport_set_int(gnutls_sd, sd);

    // define handshake tieout on gnutls socket descriptor
    gnutls_handshake_set_timeout(gnutls_sd, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);

    // initialize handshake
    int err_handshake = gnutls_handshake(gnutls_sd);

    if(err_handshake){
        tls_established = false;
        return 1;
    }

    tls_established = true;
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

// https://stackoverflow.com/questions/43264266/c-socket-send-and-connect
void pop3client::read(){
    char buff[80000]{};
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