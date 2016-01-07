
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
using namespace std;

class ClientSocket
{
      char* ip;
      char* port;
      int sockfd, rv;
      struct addrinfo hints, *servinfo, *p;
      char s[INET6_ADDRSTRLEN];

      void *get_in_addr(struct sockaddr *sa)
      {
            if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
            }
            return &(((struct sockaddr_in6*)sa)->sin6_addr);
      }

      // initialize TCP client socket and make a connection
      void initSocket()
      {
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
         ClientSocket(char* clientip, char* portno): ip{clientip}, port{portno}
         {
            initSocket();  //initialize and connect to a remote socket
         }
        ~ClientSocket() {}

        // public interface
        void sendMsg(const char* msg)
        {
            try
            {
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

