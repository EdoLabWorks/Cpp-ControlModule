

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
#include <sys/poll.h>
#include "SocketError.h"

namespace TcpClient {
using namespace std;

class ClientSocket
{
      	int sockfd, rv, rd;
      	char data[256];
      	struct addrinfo hints, *servinfo, *p;
      	char s[INET6_ADDRSTRLEN];
      	struct pollfd rs[1]; //read socket array for timeout

      	void *get_addr(struct sockaddr *sa)
      	{
            if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
            }
            return &(((struct sockaddr_in6*)sa)->sin6_addr);
      	}

      	// initialize a TCP client socket and attempt to make a connection to a remote endpoint
      	const void initSocket(char* port, char* ip)
      	{
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;

            if ((rv = getaddrinfo(ip, port, &hints, &servinfo)) != 0) {
                cout << "getaddrinfo:\n" << gai_strerror(rv) << endl;
                throw SocketError();
            }

            // attempt to connect to a remote endpoint
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
            inet_ntop(p->ai_family, get_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
            cout << "client connected to: " << s << ": " << port << endl; //debug ouput
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
        // 1st ctor, use with Connect() method
        ClientSocket() {}
        // 2nd ctor, immediately initialize the client socket with the port and ip provided
        // if no ip provided it will default to "localhost"
        ClientSocket(char* port, char* ip = "localhost")  {initSocket(port, ip);}
        virtual ~ClientSocket() {}

        // provide server port to connect with the listening server, use with 1st ctor
        // if no ip provided it will default to "localhost"
        virtual const void Connect(  char* port, char* ip = "localhost")
        {
            try
            {
                initSocket(port, ip);
            }
            catch (SocketError& e)
            {
                cerr << "Client Initialize Error: " << e.what() << endl;
                closeHandler();
            }
        }

        // send data
        virtual void Send(const char* msg) const
        {
            try
            {
                int n = send(sockfd, msg, strlen(msg), 0);
                if (n < 0) {
                    throw SocketError();
                }
            }
            catch (SocketError& e)
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
                rs[0].fd = sockfd;
                // check for normal and out-of-bound data from the socket
                rs[0].events = POLLIN | POLLPRI;
                // check socket event for available data, wait 200 milliseconds for timeout
                rd = poll(rs, 1, 200);
                if (rd < 0) {
                    //error("ERROR server reading from socket");
                    throw SocketError();
                } else if (rd == 0) {
                    cout << "Client read timeout error! No data received!\n";
                } else {
                    int n;
                    bzero(data, sizeof(data));
                    // check for events on newsockfd:
                    if (rs[0].revents & POLLIN) {
                        n = recv(sockfd, data, sizeof(data), 0); // receive normal data
                    }
                    if (rs[0].revents & POLLPRI) {
                        n = recv(sockfd, data, sizeof(data), MSG_OOB); // out-of-band data
                    }
                    if (n == 0){
                         cout << "Client read error, socket is closed or disconnected!\n";
                    }
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

