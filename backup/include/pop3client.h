/*
author: David Fischer
matnr:  i16079
file:   pop3client.h
desc:   NVS Projekt 2 - Simpler POP3 Client mit TLS Support
class:  5C
catnr:  03
*/

#pragma once

#include <iostream>
//#include <asio/ssl.hpp>
#include <asio.hpp>

class pop3client{
    private:
        std::string end_signal = ".\r";

        std::string hostname;
        std::string port;
        bool tls;
        //asio::ssl::context ssl_ctx(asio::ssl::context::tlsv12);

        asio::ip::tcp::iostream strm;


    public:
        pop3client(std::string hn, std::string prt, bool tl) : hostname( hn ), port( prt ), tls( tl ){};

        void _quit(){
            std::string data;

            strm << "NOOP" << std::endl;

            getline(strm, data);
            std::cout << data << std::endl;

            strm << "QUIT" << std::endl;

            getline(strm, data);
            std::cout << data << std::endl;
            
            strm.close();
        }

        void _connect(){
            asio::io_context ctx;
            asio::ip::tcp::resolver resolver{ctx};
            try {
                auto results = resolver.resolve(hostname, port);
                asio::ip::tcp::socket sock{ctx};

                //asio::ssl::stream<asio::ip::tcp::socket> sock(ctx, ssl_ctx);


                asio::connect(sock, results);
                
                asio::ip::tcp::iostream st{std::move(sock)};
                
                if(strm) {
                try {
                    std::string data;
                    getline(strm, data);
                    std::cout << data << std::endl;
                    
                    strm << "USER user@nvs.com" << std::endl;

                    getline(strm, data);
                    std::cout << data << std::endl;

                    strm << "PASS 12345678" << std::endl;

                    getline(strm, data);
                    std::cout << data << std::endl;

                    strm << "NOOP" << std::endl;

                    getline(strm, data);
                    std::cout << data << std::endl;

                    strm << "LIST" << std::endl;

                    while ( data != end_signal ){
                        getline(strm, data);
                        std::cout << data << std::endl;
                    }

                    data = "";

                    strm << "RETR 1" << std::endl;

                    while ( data != end_signal ){
                        getline(strm, data);
                        std::cout << data << std::endl;
                    }

                    //strm << "QUIT" << std::endl;

                    //getline(strm, data);
                    //std::cout << data << std::endl;
                    
                    //strm.close();


                } catch(std::exception e) {
                    std::cout << "err" << std::endl;
                }
            } else { 
                std::cout << "could not connect" << std::endl;
            }

            } catch(...){
                std::cout << "test" << std::endl;
            }
        };

};