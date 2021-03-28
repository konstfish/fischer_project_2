/*
author: David Fischer
matnr:  i16079
file:   main.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include <grpc/grpc.h>
#include <tabulate/table.hpp>
#include <httplib.h>
#include <json.hpp>
#include <inja.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "pop3.grpc.pb.h"
#include "POP3CSClient.h"

#include <iostream>

using namespace std;
using json = nlohmann::json;
using namespace inja;

int main() {
    auto console = spdlog::stdout_color_mt("console");

    std::string address("localhost:50051");

    spdlog::get("console")->info("Connecting to gRPC Server on {}", address);

    POP3CSClient client(
        grpc::CreateChannel(
            address, 
            grpc::InsecureChannelCredentials()
        )
    );

    // http server
    httplib::Server svr;

    // update thread pool (support 15 concurrent connections)
    svr.new_task_queue = [] { return new httplib::ThreadPool(15); };

    // customize server parameters
    svr.set_keep_alive_max_count(20);
    svr.set_keep_alive_timeout(90);

    // / route, calls the Coordinator::message_req function
    svr.Get("/", [&](const httplib::Request& req, httplib::Response& res) {
        spdlog::get("console")->info("Serving /");

        Environment env;

        pop3msg::MailList ml = client.retrieve_messages("ls", 50);
        // spdlog::info("HTTP: /req from {}", conv);

        json data;

        data["amount"] = ml.mails_size();
        
        data["emails"] = json::array();
        int i = 0;

        while(i < data["amount"]){
            data["emails"][i] = json::object();

            data["emails"][i]["message_id"] = ml.mails(i).message_id();
            data["emails"][i]["from"] = ml.mails(i).from();
            data["emails"][i]["subject"] = ml.mails(i).subject();
            data["emails"][i]["date"] = ml.mails(i).date();

            i += 1;
        }

        Template temp = env.parse_template("../templates/render.html");
        string result = env.render(temp, data);

        res.set_content(result, "text/html");
    });

    svr.Get("/del", [&](const httplib::Request& req, httplib::Response& res) {
        spdlog::get("console")->info("Serving /del");

        auto id = req.get_param_value("mid");
        
        if(id == ""){
            res.set_content("Invalid Request", "text/plain");   
        }
        
        int arg_int = stoi(id);

        pop3msg::Success suc = client.delete_message("rm", arg_int);

        res.set_content("done", "text/plain");
    });

    string http_srv_address = "127.0.0.1";
    int port = 5001;

    spdlog::get("console")->info("Starting HTTP Server on {}:{}", http_srv_address, port);

    svr.listen("127.0.0.1", port);
}