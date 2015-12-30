// -*- C++ -*-
/*
* File:   TcpServer.h
* Author: Ed Alegrid
*
*/

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

namespace TcpServer{

class ServerSocket
{
      int sockfd, newsockfd;
      int portno;
      socklen_t clilen;
      char message[256];
      struct sockaddr_in serv_addr, cli_addr;
      int n;

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
	        bzero((char *) &serv_addr, sizeof(serv_addr));
	        serv_addr.sin_family = AF_INET;
	        serv_addr.sin_addr.s_addr = INADDR_ANY; //accepts all client ip
	        serv_addr.sin_port = htons(portno);
	        // bind the socket the host IP address with specified port
	        // if port is already used, it will be reused again on next startup
             	int yes = 1;
             	int bindStatus = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	        bindStatus = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	        if ( bindStatus < 0) {error("ERROR on binding");}
	        // cout << "binding the socket ..." << endl; //debug output
	        // set the maximum size for the backlog queue
	        listen(sockfd, 10);
	        cout << "server listening on port " << portno << endl;
	        // accept() call will now wait for incoming connection
	        clilen = sizeof(cli_addr);
	        cout << "press Ctrl-C to stop the server\n" << endl;
        }
        catch (exception& e)
  	{
		 cerr << e.what() << endl;
  	}
      }

      public:
        ServerSocket() {}
        ~ServerSocket() {}

      void createSocket(int port){
            portno = port;
            initSocket();
      }
      // accept new client from the socket
      void acceptSocket()
      {
            try
            {
                newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
                if (newsockfd < 0){error("ERROR on accept");}
            }
            catch (exception& e)
            {
                cerr << e.what() << endl;
            }
      }
      // read data from connected client, use only acceptSocket method
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
