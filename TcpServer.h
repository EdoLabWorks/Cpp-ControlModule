
// -*- C++ -*-
/*
* File:   TcpServer.h
* Author: Ed Alegrid
*
*/

#pragma once
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

namespace TcpServer {
using namespace std;

class ServerSocket
{
      int sockfd, newsockfd;
      int port, n;
      socklen_t clen;
      char message[256];
      struct sockaddr_in server_addr, client_addr;

[[noreturn]] void error(const char *msg)
      {
            perror(msg);
            close(newsockfd);
            close(sockfd);
            exit(1);
      }

      void initSocket()
      {
     	try
     	{
	        // create a TCP server socket
	        sockfd =  socket(AF_INET, SOCK_STREAM, 0);
	        if (sockfd < 0) {error("ERROR opening socket");}
	        // cout << "creating a TCP socket ..." << endl; //debug output
	        // clear address structure
	        bzero((char *) &server_addr, sizeof(server_addr));
	        server_addr.sin_family = AF_INET;
	        server_addr.sin_addr.s_addr = INADDR_ANY; //accepts all client ip
	        server_addr.sin_port = htons(port);
	        // bind the socket with the host IP address with the specified port
	        // if port is already used, it will be re-used again on next startup
            int yes = 1;
            int bindStatus = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	        bindStatus = bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	        if ( bindStatus < 0) {error("ERROR on binding");}
	        // cout << "binding the socket ..." << endl; //debug output
	        // set the maximum size for the backlog queue
	        listen(sockfd, 5);
	        cout << "server listening on port " << port << endl;

	        clen = sizeof(client_addr);
	        cout << "press Ctrl-C to stop the server\n" << endl;
        }
        catch (exception& e)
        {
            cerr << e.what() << endl;
        }
      }

      public:
         ServerSocket(int &portno): port{portno}
         {
            initSocket(); //initialize, bind and start listening
         }
        ~ServerSocket() {}

      // accept new client from the socket
      void acceptSocket()
      {
            try
            {
                // accept() call will wait for new client
                newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clen);
                if (newsockfd < 0){error("ERROR on accept");}
	    }
            catch (exception& e)
            {
                cerr << e.what() << endl;
            }
      }
      // read data from connected client, use only after calling acceptSocket() method
      const char* readData()
      {
            try
            {
                bzero(message, 256);
                n = read(newsockfd, message, 256);
                if (n < 0) {error("ERROR reading from socket");}

                close(newsockfd);
            }
            catch (exception& e)
            {
                cerr << e.what() << endl;
            }
            return message;
      }
};

}
