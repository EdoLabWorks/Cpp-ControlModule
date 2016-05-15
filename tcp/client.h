/*
* File:   client.h
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
#include "socketerror.h"

namespace Tcp {

class Client
{
      int sockfd, rv, rd;
      char data[1024];
      char s[INET6_ADDRSTRLEN];
      struct pollfd rs[1];

      void *get_addr(struct sockaddr *sa)
      {
            if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
            }
            return &(((struct sockaddr_in6*)sa)->sin6_addr);
      }

      int initSocket(const int &port, const std::string &ip)
      {
            std::string PORT;
            std::stringstream out;
            out << port;
            PORT = out.str();

            addrinfo hints{};
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            addrinfo *servinfo{};

            try{

                if (port <= 0){
                    throw SocketError("Invalid port");
                }

                if ((rv = getaddrinfo(ip.c_str(), PORT.c_str(), &hints, &servinfo)) != 0) {
                    std::cout << "getaddrinfo: " << gai_strerror(rv) << std::endl;
                    throw SocketError("Invalid address");
                }

                sockfd = {socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)};
                int result{ connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)};
                if (result < 0)
                {
                    std::cout << "Client connection failed!" << std::endl;
                    throw SocketError();
                }
                else
                {
                    // details of remote connected endpoint
                    inet_ntop(servinfo->ai_family, get_addr((struct sockaddr *)servinfo->ai_addr), s, sizeof s);
                    // initial client console output, provide one in your application
                    std::cout << "Client connected to: " << s << ":" << port << std::endl; //debug ouput
                }

		if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0){ throw SocketError();}

                if (servinfo == nullptr) {
                    throw SocketError("Client connect fail ...");
                }

                freeaddrinfo(servinfo);

                rs[0].fd = sockfd;
                rs[0].events = POLLIN | POLLPRI;

                return 0;

            }

            catch (SocketError& e)
            {
                std::cerr << "Client Socket Initialize Error: " << e.what() << std::endl;
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

       Client() {}
       Client(const int &port, const std::string &ip = "127.0.0.1")  {initSocket(port, ip);}
       virtual ~Client() {}

       virtual const void Connect(const int &port, const std::string &ip = "127.0.0.1")
       {
           initSocket(port, ip);
       }

       virtual const std::string Send(const std::string &msg)
       {
           try
           {
               int n = send(sockfd, msg.c_str(), strlen(msg.c_str()), 0);
               if (n < 0) {
                   throw SocketError();
               }
           }
           catch (SocketError& e)
           {
                std::cerr << "Client Send Error: " << e.what() << std::endl;
                closeHandler();
           }
           return msg;
       }

       virtual const std::string Read()
       {
            try
            {
                // check socket event for available data, wait 200 milliseconds for timeout
                rd = poll(rs, 1, 200);
                if (rd < 0) {
                    throw SocketError();
                } else if (rd == 0) {
                    std::cout << "Client read timeout error! No data received!\n";
                } else {
                    ssize_t n{1};
                     data[1024] = {};
                    bzero(data, sizeof(data));
                    // check for events on newsockfd:
                    if (rs[0].revents & POLLIN) {
                        rs[0].revents = 0;
                        n = {recv(sockfd, data, sizeof(data), 0)}; // receive normal data
                    }
                    if (rs[0].revents & POLLPRI) {
                        rs[0].revents = 0;
                        n = {recv(sockfd, data, sizeof(data), MSG_OOB)}; // out-of-band data
                    }
                    if (n == 0){
                         std::cout << "Client read error, socket is closed or disconnected!\n";
                    }
                }
            }
            catch (SocketError& e)
            {
                std::cerr << "Client Read Error: " << e.what() << std::endl;
                closeHandler();
            }
            return data;
       }

       virtual void Close()
       {
            close(sockfd);
       }
};

}

