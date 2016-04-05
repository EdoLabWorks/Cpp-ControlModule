/*
* File:   server.h
* Author: Ed Alegrid
*
*/

#pragma once
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <sstream>
#include <netdb.h>
#include "socketerror.h"

namespace Tcp {
using namespace std;

class Server
{
    int sockfd, newsockfd, PORT, rv;
    string IP;
    socklen_t clen;
    char data[1024];
    struct sockaddr_in server_addr, client_addr;
    int listenF = false;
    int ServerLoop = false;
    struct pollfd rs[1];

// C-style error handler
[[noreturn]] void error(const char *msg)
    {
        perror(msg);
        close(newsockfd);
        close(sockfd);
        delete this;
        exit(1);
    }

    int initSocket(const int &port, const string ip = "127.0.0.1")
    {
    PORT = port;
    IP = ip;
    try
    {
        if (port <= 0){
                throw SocketError("Invalid port");
        }
        sockfd =  socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
                throw SocketError();
        }

        bzero((char *) &server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        inet_aton(ip.c_str(), &server_addr.sin_addr);
        server_addr.sin_port = htons(port);

        int yes = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if ( bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        {
            throw SocketError();
        }
        else
        {
            listen(sockfd, 5);
            clen = sizeof(client_addr);
        }
        return 0;
    }
    catch (SocketError& e)
    {
        cerr << "Server Socket Initialize Error: " << e.what() << endl;
        closeHandler();
        exit(1);
    }
    }

    void closeHandler() const
    {
        Close();
        delete this;
        exit(1);
    }

    public:
    // use with createServer() method
    Server(){}
    // immediately initialize the server socket with the port provided
    Server(const int &port, const string ip = "127.0.0.1" ): PORT{port}, IP{ip} { initSocket(port); }
    virtual ~Server() {}

    void createServer(const int &port, const string ip = "127.0.0.1")
    {
        initSocket(port, ip);
    }

    // listen and accept new client from the socket
    void Listen(int serverloop = false)
    {
        ServerLoop = serverloop;

        if (!listenF){
            // initial server console output, provide one in your application
            //cout << "server listening on port: " << inet_ntoa(server_addr.sin_addr) << ":" << PORT << "\n\n"; //debug output
            cout << "Server listening on: " << IP << ":" << PORT << "\n\n"; //debug output
            listenF = true;
        }

        try
        {
            newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clen);
            if (newsockfd < 0){
                throw SocketError();
            }
            //cout << "server connection from client " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << "\n\n"; //debug output
            rs[0].fd = newsockfd;
            rs[0].events = POLLIN | POLLPRI;

        }
        catch (SocketError& e)
        {
            cerr << "Server Listen Error: " << e.what() << endl;
            closeHandler();
        }
    }

    virtual const string Read()
    {
        try
        {
            // check socket event for available data, wait 100 milliseconds for timeout
            rv = poll(rs, 1, 100); //adjust timeout for your requirements
            if (rv < 0) {
                throw SocketError();
            } else if (rv == 0) {
                cout << "Server read timeout error! No data received!\n";
            } else {
                ssize_t n{1};
                bzero(data, sizeof(data));
                // check for events on newsockfd:
                if (rs[0].revents & POLLIN) {
                    rs[0].revents = 0;
                    n = {recv(newsockfd, data, sizeof(data), 0)}; // receive normal data
                }
                if (rs[0].revents & POLLPRI) {
                    rs[0].revents = 0;
                    n = {recv(newsockfd, data, sizeof(data), MSG_OOB)}; // out-of-band data
                }
                if (n == 0){
                     cout << "Server read error, socket is closed or disconnected!\n";
                }
            }
        }
        catch (SocketError& e)
        {
            cerr << "Server Read Error: " << e.what() << endl;
            closeHandler();
        }
        return data;
    }

    virtual string Send(const string msg) const
    {
        try
         {
            ssize_t n{send(newsockfd, msg.c_str(), strlen(msg.c_str()), 0)};
            if (n < 0) {
                throw SocketError();
            }
        }
        catch (SocketError& e)
        {
            cerr << "Server Send Error: " << e.what() << endl;
            closeHandler();
        }

        return msg;
    }

    virtual void Close() const
    {
        if(ServerLoop){
            close(newsockfd);
        }
        else{
            close(newsockfd);
            close(sockfd);
        }
    }
};

}
