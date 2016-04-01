
/*
 * File:   TcpServer.h
 * Author: Ed Alegrid
 *
 */

#pragma once
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include "SocketError.h"
#include <sys/poll.h>

namespace TcpServer {
using namespace std;

class ServerSocket
{
      int sockfd, newsockfd, PORT, rv;
      socklen_t clen;
      char data[256];
      struct sockaddr_in server_addr, client_addr;
      int listenF = false;
      int ServerLoop = false;
      struct pollfd rs[1]; //read poll array for timeout

// C-style error handler
[[noreturn]] void error(const char *msg)
      {
            perror(msg);
            close(newsockfd);
            close(sockfd);
            delete this;
            exit(1);
      }

      void initSocket(const int &port)
      {
      PORT = port;
     	try
     	{
            // create a TCP server socket
            sockfd =  socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                    //error("ERROR opening socket"); // C-style error handler
                    throw SocketError(); // C++ error handler
            }

            bzero((char *) &server_addr, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY;
            server_addr.sin_port = htons(port);

            int yes = 1;
            int BindStatus = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	      BindStatus = bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	      if ( BindStatus < 0)
            {
                    //error("ERROR on binding");
                    throw SocketError();
            }
            else
            {
                    listen(sockfd, 5);
                    clen = sizeof(client_addr);
            }
        }
        catch (SocketError& e)
        {
            cerr << "Server Initialize Error: " << e.what() << endl;
            closeHandler();
        }
      }

      // cleanup method
      void closeHandler() const
      {
            Close();
            delete this;
            exit(1);
      }

      public:
      // 1st ctor, use with createServer() method
      ServerSocket(){}

      // 2nd ctor, immediately initialize the server socket with the port provided
      // ip defaults to host computer ip
      ServerSocket(const int &port): PORT{port} { initSocket(port); }
      virtual ~ServerSocket() {}

      // provide port to initialize server, use with 1st ctor
      // ip defaults to host computer ip
      void createServer(const int &port)
      {
            initSocket(port);
      }

      // listen and accept new client from the socket
      void Listen(int serverloop = false)
      {
            ServerLoop = serverloop;

            if (!listenF){
                // initial console output, provide one in your main app
                cout << "server listening on port: " << PORT << "\n\n"; //debug output
                listenF = true;
            }

            try
            {
                // Listen() call will wait for new client
                newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clen);
                if (newsockfd < 0){
                    //error("ERROR on accept");
                    throw SocketError();
                }

            }
            catch (SocketError& e)
            {
                cerr << "Server Listen Error: " << e.what() << endl;
                closeHandler();
            }

      }

      // read data, use only after calling Listen() call
      virtual const char* Read()
      {
            try
            {
                rs[0].fd = newsockfd;
                // check for normal and out-of-bound data from the socket
                rs[0].events = POLLIN | POLLPRI;
                // check socket event for available data, wait 100 milliseconds for timeout
                rv = poll(rs, 1, 100); //adjust timeout for your requirements
                if (rv < 0) {
                    //error("ERROR server reading from socket");
                    throw SocketError();
                } else if (rv == 0) {
                    cout << "Server read timeout error! No data received!\n";
                } else {
                    int n;
                    bzero(data, sizeof(data));
                    // check for events on newsockfd:
                    if (rs[0].revents & POLLIN) {
                        rs[0].revents = 0;
                        n = recv(newsockfd, data, sizeof(data), 0); // receive normal data
                    }
                    if (rs[0].revents & POLLPRI) {
                        rs[0].revents = 0;
                        n = recv(newsockfd, data, sizeof(data), MSG_OOB); // out-of-band data
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

      // send data, use only after calling Listen() call
      virtual void Send(const char* msg) const
      {
            try
            {
                int n = send(newsockfd, msg, strlen(msg), 0);
                if (n < 0) {
                    //error("ERROR server sending to socket");
                    throw SocketError();
                }
            }
            catch (SocketError& e)
            {
                cerr << "Server Send Error: " << e.what() << endl;
                closeHandler();
            }
      }

      // close socket
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
