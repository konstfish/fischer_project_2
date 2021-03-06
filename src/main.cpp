/*
author: David Fischer
matnr:  i16079
file:   main.cpp
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#include "pop3client.h"

#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;

int main() {
    auto console = spdlog::stdout_color_mt("console");
    auto logger = spdlog::basic_logger_mt("logger", "logs/basic-log.txt");

    spdlog::get("logger")->info("Starting POP3 Client");

    //pop3client c("fortimail.konst.fish", 110, false);
    pop3client c("fortimail.konst.fish", 995, true);
    c.establish_connection();
    c.login("user@nvs.com", "12345678");

    c.get_total_messages();
    //c.delete_message(5);
    c.retrieve_messages(4);

    c.debug();

    c.quit();

    return 0;
}