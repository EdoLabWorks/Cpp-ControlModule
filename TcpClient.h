
/*
 * File:   TcpClient.h
 * Author: Ed Alegrid
 *
 */

#pragma once
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include "SocketError.h"

namespace TcpClient {
using namespace std;

class ClientSocket
{
      char* ip;
      char* port;
      int sockfd, rv;
      char data[256];
      struct addrinfo hints, *servinfo, *p;
      char s[INET6_ADDRSTRLEN];

      void *get_in_addr(struct sockaddr *sa)
      {
            if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
            }
            return &(((struct sockaddr_in6*)sa)->sin6_addr);
      }

      // initialize a TCP client socket and attempt to make a connection to a remote endpoint
      void initSocket()
      {
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;

            if ((rv = getaddrinfo(ip, port, &hints, &servinfo)) != 0) {
                cout << "getaddrinfo:\n" << gai_strerror(rv) << endl;
                throw SocketError();
            }

            // connect to a remote endpoint
            int n = 0;
            for(p = servinfo; p != NULL; p = p->ai_next) {

                if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                     throw SocketError();
                }

                if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                     close(sockfd);
                     n++; cout << n << " try" << endl;
                     throw SocketError();
                }
                break;
            }

            if (p == NULL) {
                 throw SocketError("client connect fail ...");
            }
            // details of remote connected endpoint
            inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
            //cout << "client connected to: " << s << ": " << port << endl; //debug ouput
            freeaddrinfo(servinfo);
      }

      // cleanup method
      void closeHandler() const
      {
            Close();
            delete this;
            exit(1);
      }

      public:
        ClientSocket(char* clientip, char* portno): ip{clientip}, port{portno}
        {
            try
            {
                initSocket();
            }
            catch (SocketError& e)
            {
                cerr << "Client Initialize Error: " << e.what() << endl;
                closeHandler();
            }
        }

        virtual ~ClientSocket() {}

        // send data
        virtual void Send(const char* msg) const
        {
            try
            {
                int len;
                len = strlen(msg);
                int n = send(sockfd, msg, len, 0);
                if (n < 0) {
                    throw SocketError();
                }
            }
            catch (exception& e)
            {
                 cerr << "Client Send Error: " << e.what() << endl;
                 closeHandler();
            }
        }

       // read data
       virtual const char* Read()
       {
            try
            {
                bzero(data, sizeof(data));
                int n = read(sockfd, data, sizeof(data));
                if (n < 0) {
                    throw SocketError();
                }
            }
            catch (SocketError& e)
            {
                cerr << "Client Read Error: " << e.what() << endl;
                closeHandler();
            }
            return data;
       }

       // close socket
       virtual void Close() const
       {
            close(sockfd);
       }

};

}

