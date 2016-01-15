
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

namespace TcpServer {
using namespace std;

class ServerSocket
{
      int sockfd, newsockfd, port;
      socklen_t clen;
      char data[256];
      struct sockaddr_in server_addr, client_addr;
      int listenF = false;
      int ServerLoop = false;

// C-style error handler
[[noreturn]] void error(const char *msg)
      {
            perror(msg);
            close(newsockfd);
            close(sockfd);
            delete this;
            exit(1);
      }

      void initSocket(const int &portno)
      {
        port = portno;
     	try
     	{
            // create a TCP server socket
            sockfd =  socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                    //error("ERROR opening socket"); // for C-style error handler
                    throw SocketError(); // for C++ error handler
            }

            bzero((char *) &server_addr, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY;
            server_addr.sin_port = htons(port);

            int yes = 1;
            int bindStat = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
            bindStat = bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
            if ( bindStat < 0)
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
         ServerSocket(){} // use with createServer method
         ServerSocket(const int &portno): port{portno} { initSocket(portno); } // option to supply port and start socket initialization
         virtual ~ServerSocket() {}

      // provide port, initialize, bind, start listening for client connection
      void createServer(const int &portno)
      {
            initSocket(portno);
      }

      // listen and accept new client from the socket
      void Listen(int serverloop = false)
      {
            ServerLoop = serverloop;

            if (!listenF){
                // initial console output, provide one in your main app
                cout << "server listening on port: " << port << "\n\n"; //debug output
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
                bzero(data, sizeof(data));
                int n = recv(newsockfd, data, sizeof(data), 0);
                if (n < 0) {
                    //error("ERROR server reading from socket");
                    throw SocketError();
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
                int len;
                len = strlen(msg);
                int n = send(newsockfd, msg, len, 0);
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
