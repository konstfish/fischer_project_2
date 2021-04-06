/*
author: David Fischer
matnr:  i16079
file:   POP3client.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "POP3client.h"

using namespace std;

// helper function to establish connection to the POP3 Server.
// resolves hostname, sets up a socket (established gnutls handshake)
int POP3client::establish_connection(){
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

    // wait for server response
    usleep(100000);

    string res = read_to_str();
    vector<string> res_vec = utility.split(res, " ");

    if(res_vec[0] == "+OK"){
        spdlog::get("console")->info("Connection successfully established, TLS: {}", tls);
        spdlog::get("logger")->info("Connection successfully established, TLS: {}", tls);
    }else{
        spdlog::get("console")->error("Unable to connect");
        spdlog::get("logger")->error("Unable to connect");
        return 1;
    }

    return 0;
}

// https://stackoverflow.com/questions/52727565/client-in-c-use-gethostbyname-or-getaddrinfo
// https://stackoverflow.com/questions/38002016/problems-with-gethostbyname-c
// helper function to resolve hostnames to an address range & ip
int POP3client::resolve() {
    hostent *rh = gethostbyname(hostname.c_str());
    if(rh == NULL){
        return 1;
    }

    in_addr **ipptr = (struct in_addr**)rh->h_addr_list;

    addrfamily  = rh->h_addrtype;
    ipaddr      = inet_ntoa(*ipptr[0]);
    return 0;
}

// http://www.hep.by/gnu/gnutls/Helper-functions-for-TCP-connections.html#Helper-functions-for-TCP-connections
// helper function to set up a socket
int POP3client::socket_setup(){
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
// helper function to establish gnutls handshake using the established socket
int POP3client::gnutls_setup(){
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
    gnutls_priority_set_direct (gnutls_sd, "NORMAL", &err);

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

// helper function to destroy the gnutls socket
void POP3client::gnutls_destruction(){
    gnutls_bye (gnutls_sd, GNUTLS_SHUT_RDWR);

    gnutls_deinit (gnutls_sd);
    gnutls_certificate_free_credentials (xcred);
    gnutls_global_deinit ();
}

// helper function to destroy the socket
void POP3client::socket_destruction(){
  shutdown (sd, SHUT_RDWR);
  close (sd);
}

// https://stackoverflow.com/questions/43264266/c-socket-send-and-connect
// read from the socket (used for simple +OK answers)
void POP3client::read(){
    char buff[8000]{};
    memset(buff, 0, sizeof(buff));

    if(tls){
        gnutls_record_recv(gnutls_sd, buff, sizeof(buff));
    }else{
        recv(sd, buff, sizeof(buff), 0);
    }

    string rec = buff;
}

// read from socket & returns a string (used for simple +OK answers)
string POP3client::read_to_str(){
    char buff[8000]{};
    memset(buff, 0, sizeof(buff));

    if(tls){
        gnutls_record_recv(gnutls_sd, buff, sizeof(buff));
    }else{
        recv(sd, buff, sizeof(buff), 0);
    }

    string rec = buff;
    return rec;
}

// When longer messages are returned, POP3 Servers end them with an end signal. 
// Specified in this function as ".\r\n". The function reads until this signal is detected.
string POP3client::read_to_end(){
    char buff[8000]{};
    memset(buff, 0, sizeof(buff));

    string key = ".\r\n";
    string rec = "";
    string tmp = "";

    while(!utility.ends_with(tmp, key)){
        if(tls){
            gnutls_record_recv(gnutls_sd, buff, sizeof(buff));
        }else{
            recv(sd, buff, sizeof(buff), 0);
        }

        tmp = buff;
        memset(buff, 0, sizeof(buff));

        rec += tmp;
    }

    return rec;
}

// https://stackoverflow.com/questions/43264266/c-socket-send-and-connect
// helper function to write into the socket
int POP3client::write(std::string msg){
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

// Reads total messages from the POP3 Server using the "STAT" function
int POP3client::get_total_messages(){
    write("STAT");
    string res = read_to_str();
    vector<string> res_vec = utility.split(res, " ");
    
    int amount = stoi(res_vec[1]);

    spdlog::get("logger")->info("Server returned {} messages.", amount);

    return amount;
}

// Deletes a message from the POP3 Server using the "DELE" function
int POP3client::delete_message(int message_id){
    write("DELE " + to_string(message_id));
    string res = read_to_str();
    vector<string> res_vec = utility.split(res, " ");
    
    if(res_vec[0] != "+OK"){
        spdlog::get("logger")->info("Unable to delete message with ID: {}", message_id);
        return 1;
    }else{
        spdlog::get("logger")->info("Deleted message with ID: {}", message_id);
    }

    return 0;
}

// Creates a vector of a vector of strings, containing a specified amount of mails
// (subject line, author, date recieved, id)
vector<vector<string>> POP3client::retrieve_messages(int amount){
    int total_messages = get_total_messages();
    vector<vector<string>> messages;

    if(amount > total_messages){
        amount = total_messages;
    }

    int retrieve = 0;

    int cur_message = total_messages;
    while(cur_message > (total_messages - amount)){
        vector<string> res_vec = retrieve_message_metadata(cur_message);

        if(res_vec[0] == "MARKED FOR DELETION"){
            retrieve += 1;
        }

        messages.push_back(res_vec);

        cur_message -= 1;
    }

    int i = 1;
    while(i <= retrieve){
        vector<string> res_vec = retrieve_message_metadata(total_messages + i);
        //messages.push_back(res_vec);
        messages.insert(messages.begin(), 1, res_vec);

        i += 1;
    }

    // utility.print_messages(messages);

    return messages;
}

// retrieves the subject, author and date from the specified email
vector<string> POP3client::retrieve_message_metadata(int message_id){
    write("TOP " + to_string(message_id));
    string res = read_to_end();

    vector<string> res_vec;

    if(res[0] == '-') {
        int i = 0;
        while(i < 3){
            res_vec.push_back("MARKED FOR DELETION");
            i += 1;
        }
    }else{
        res_vec = utility.split_message(res);
    }

    res_vec.push_back(to_string(message_id));

    return res_vec;
}

// helper function to attempt a login on the pop3 server
int POP3client::login(string user, string password){

    write("USER " + user);
    read();
    write("PASS " + password);

    string res = read_to_str();
    vector<string> res_vec = utility.split(res, " ");

    if(res_vec[0] == "+OK"){
        spdlog::get("console")->info("Successfully Logged in!");
        spdlog::get("logger")->info("Successfully Logged in!");
    }else{
        spdlog::get("console")->info("Invalid User Data!");
        spdlog::get("logger")->info("Invalid User Data!");
        return 1;
    }

    return 0;
}

// disconnects from the pop3 server
int POP3client::quit(){
    write("QUIT ");

    string res = read_to_str();
    vector<string> res_vec = utility.split(res, " ");

    if(res_vec[0] == "+OK"){
        spdlog::get("console")->info("Successfully Quit Session!");
        spdlog::get("logger")->info("Successfully Quit Session!");
    }else{
        spdlog::get("console")->info("Error while quitting Session!");
        spdlog::get("logger")->info("Error while quitting Session!");
        exit(1);
    }

    exit(0);
}

// saves email as .eml file (in the build folder)
int POP3client::save_mail(int message_id){
    write("RETR " + to_string(message_id));
    string email = read_to_end();

    vector<string> res_vec = retrieve_message_metadata(message_id);

    int check{1};

    check = utility.email_to_file(email, res_vec[1]);
    if(check){
        spdlog::get("logger")->info("Unable to save message with ID: {} as {}", message_id, res_vec[1]);
        return 1;
    }else{
        spdlog::get("logger")->info("Saved message with ID: {} as {}", message_id, res_vec[1]);
    }

    return 0;
}
