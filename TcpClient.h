// -*- C++ -*-
/*
 * File:   TcpClient.h
 * Author: Ed Alegrid
 *
 */

#pragma once
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

namespace TcpClient
{

class ClientSocket
{
      char* ip;
      char* port;
      int sockfd;
      struct addrinfo hints, *servinfo, *p;
      int rv;
      char s[INET6_ADDRSTRLEN];

      void *get_in_addr(struct sockaddr *sa)
      {
            if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
            }
            return &(((struct sockaddr_in6*)sa)->sin6_addr);
      }

      // initialize TCP client socket and make a connection
      void initSocket(char* ip, char* port)
      {
            this->ip = ip;
            this->port = port;
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;

            if ((rv = getaddrinfo(ip, port, &hints, &servinfo)) != 0) {
                cout << "getaddrinfo:\n" << gai_strerror(rv) << endl;
            }
            // loop through all the results and attempt to connect
            int n = 0;
            for(p = servinfo; p != NULL; p = p->ai_next) {

                if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                    perror("client socket");
                    continue;
                }

                if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                     close(sockfd);
                     n++; cout << n << " try" << endl;
                     perror("client connect");
                    continue;
                }
                break;
            }

            if (p == NULL) {
                cout << "\nclient failed to connect ..." << endl;
            }
            inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);

            // cout << "client connected to: " << s << ": " << port << endl; //debug ouput
            freeaddrinfo(servinfo);
      }

      public:
        ClientSocket() {}
        ~ClientSocket() {}
        // public interface
        void sendMsg(char* ip, char* port, const char* msg)
        {
            // initialize and connect to a remote socket
            try
            {
                initSocket(ip, port);
                int len;
                len = strlen(msg);
                send(sockfd, msg, len, 0);

                close(sockfd);
            }
            catch (exception& e)
            {
                cerr << e.what() << endl;
            }
        }
};

}


