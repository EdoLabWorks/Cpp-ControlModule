// -*- C++ -*-
/*
* File:   TcpServer.h
* Author: Ed Alegrid
*
* This is just a standard TCP socket based on Beej's Guide to Network Programming.
* We are just trying to re-structure it for object-oriented programming in C++. 
*/

#pragma once
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

namespace TcpServer{

class ServerSocket
{
      int sockfd, newsockfd;
      int port, n;
      socklen_t clilen;
      char message[256];
      struct sockaddr_in serv_addr, cli_addr;
      
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
	        serv_addr.sin_port = htons(port);
	        // bind the socket with the host IP address with the specified port
	        // if port is already used, it will be re-used again on next startup
             	int yes = 1;
             	int bindStatus = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	        bindStatus = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	        if ( bindStatus < 0) {error("ERROR on binding");}
	        // cout << "binding the socket ..." << endl; //debug output
	        // set the maximum size for the backlog queue
	        listen(sockfd, 5);
	        cout << "server listening on port " << port << endl;
	       
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
            this->port = port;
            initSocket();
      }
      // accept new client from the socket
      void acceptSocket()
      {
            try
            {
                // accept() call will wait for new client
                newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
                if (newsockfd < 0){error("ERROR on accept");}
                // cout << "newsockfd: " << newsockfd << endl; //debug output, should be 4 always
                // we can re-use it or assign it to a new child process in an infinite loop
                // here we will just re-use it and close it after read or send process 
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
		// close socket after reading data
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
