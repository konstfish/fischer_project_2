/*
author: David Fischer
matnr:  i16079
file:   main.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "POP3client.h"

#include "CLI11.hpp"

#include <json.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/basic_file_sink.h"

#include <iostream>

using namespace std;
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    // CLI11 setup
    CLI::App app("POP3 Client");

    // Connection Options
    string servername{""};
    app.add_option("-s,--server", servername, "Server Domain");

    string username{""};
    app.add_option("-u,--user", username, "Account Name");

    string password{""};
    app.add_option("-p,--pass", password, "Account Password");

    bool use_tls{false};
    app.add_flag("-t, --tls", use_tls, "Connect using TLS");

    int port{-1};
    app.add_option("--port", port, "Specify custom port");

    string jsonfile{""};
    app.add_option("-j,--json", jsonfile, "Relative path to JSON file containing the Config");

    // Commands to execute
    int download{-1};
    app.add_option("-d,--download", download, "Download Email with specified ID");

    int del{-1};
    app.add_option("-r,--remove", del, "Delete Email with specified ID");

    int list{-1};
    app.add_option("-l,--list", list, "List ")
        ->check(CLI::Range(1,50).description("Range of Emails limited to sensible values").active(true).name("range"));

    // Enable interactive Shell

    // TODO

    // Enable web interface

    // TODO

    // PARSE argv
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) { 
        return app.exit(e);
    }

    // Prase json, if specified
    if(jsonfile != ""){
        std::ifstream i("../" + jsonfile);
        json j;
        i >> j;
        //cout << j << endl;

        servername = j["hostname"];
        username = j["username"];
        password = j["password"];
        use_tls = j["tls"];
        port = j["port"];
    }

    // set port dynamically if not specified
    if(port == -1){
        if(use_tls){
            port = 995;
        }else{
            port = 110;
        }
    }

    // set up spdlog
    auto console = spdlog::stdout_color_mt("console");
    auto logger = spdlog::basic_logger_mt("logger", "logs/basic-log.txt");

    spdlog::get("logger")->info("Starting POP3 Client");

    //pop3client c("fortimail.konst.fish", 110, false);
    //pop3client c("fortimail.konst.fish", 995, true);

    // create pop3client
    POP3client c(servername, port, use_tls);
    int check = 0;
    
    check = c.establish_connection();
    if(check){
        return 1;
    }

    check = c.login(username, password);
    if(check){
        return 1;
    }

    // check for commands
    if(download != -1){
        c.save_mail(download);
    }

    if(list != -1){
        c.retrieve_messages(list);
    }

    if(del != -1){
        c.delete_message(del);
    }

    c.quit();

    return 0;
}