/*
* File:   server.h
* Author: Ed Alegrid
*
*/

#pragma once
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <sstream>
#include <netdb.h>
#include <sys/fcntl.h>
#include <thread>
#include <future>
#include "socketerror.h"

namespace Tcp {

class Server
{
    int sockfd, newsockfd, PORT, rv;
    std::string IP;
    socklen_t clen;
    char data[1024];
    sockaddr_in server_addr{}, client_addr{};
    int listenF = false;
    int ServerLoop = false;
    struct pollfd rs[1];

    int initSocket(const int &port, const std::string &ip = "127.0.0.1")
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
	  server_addr.sin_family = AF_INET;
	  server_addr.sin_port = htons(port);
	  inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

	  int reuse = 1;
	  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
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
	   std::cerr << "Server Socket Initialize Error: " << e.what() << std::endl;
	   closeHandler();
	   exit(1);
	}
    }

    void closeHandler()
    {
        Close();
        delete this;
        exit(1);
    }

    public:

    Server(){}
    Server(const int &port, const std::string &ip = "127.0.0.1" ): PORT{port}, IP{ip} { initSocket(port, ip); }
    virtual ~Server() {}

    void createServer(const int &port, const std::string &ip = "127.0.0.1")
    {
        initSocket(port, ip);
    }

    void Listen(int serverloop = false)
    {
        ServerLoop = serverloop;

        try
        {
            auto l = [] (int fd, sockaddr_in client_addr, socklen_t clen)
            {
               auto newfd = accept4(fd, (struct sockaddr *) &client_addr, &clen, SOCK_NONBLOCK);
               if (newfd < 0){ throw SocketError("Invalid socket descriptor! Listen flag is false! \nMaybe you want to set it to true like Listen(true).");}
               return newfd;
            };


        if (!listenF){
            // initial server console output, provide one in your application
            //std::cout << "server listening on port: " << inet_ntoa(server_addr.sin_addr) << ":" << PORT << "\n\n"; //debug output
            std::cout << "Server listening on: " << IP << ":" << PORT << "\n\n"; //debug output
            listenF = true;
        }

            auto nfd = std::async(l, sockfd, client_addr, clen);
            newsockfd = nfd.get();

            //std::cout << "server connection from client " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << "\n\n"; //debug output
            rs[0].fd = newsockfd;
            rs[0].events = POLLIN | POLLPRI;

        }
        catch (SocketError& e)
        {
            std::cerr << "Server Listen Error: " << e.what() << std::endl;
            closeHandler();
        }
    }

    virtual const std::string Read()
    {
        try
        {
            if(!listenF){
                 throw SocketError("No listening socket!\n Did you forget to start the Listen() method!");
            }

            // check socket event for available data, wait 10 milliseconds for timeout
            rv = poll(rs, 1, 10); //adjust timeout for your requirements
            if (rv < 0) {
                throw SocketError();
            } else if (rv == 0) {
                std::cout << "Server read timeout error! No data received!\n";
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
                     std::cout << "Server read error, socket is closed or disconnected!\n";
                }
            }
        }
        catch (SocketError& e)
        {
            std::cerr << "Server Read Error: " << e.what() << std::endl;
            closeHandler();
        }
        return data;
    }

    virtual std::string Send(const std::string &msg)
    {
        try
         {
            if(!listenF){
                 throw SocketError("No listening socket!\n Did you forget to start the Listen() method!");
            }

            ssize_t n{send(newsockfd, msg.c_str(), strlen(msg.c_str()), 0)};
            if (n < 0) {
                throw SocketError();
            }
        }
        catch (SocketError& e)
        {
            std::cerr << "Server Send Error: " << e.what() << std::endl;
            closeHandler();
        }

        return msg;
    }

    virtual void Close()
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
