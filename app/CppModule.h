/*
* File:   Controller.h
* Author: Ed Alegrid
*
*/

#pragma once
#include <iostream>
#include <memory>
#include "../tcp/server.h"
#include "../tcp/client.h"
#include "device.h"

using namespace std;
class App
{
    public:
    
        /*******************************************
    	* a simple tcp server using smart pointers *
        ********************************************/
        static void startCtrl()
        {
            cout << "\n*** C++ IO-Control Project Using Smart Pointers for Dynamic Memory ***\n" << endl;

            // create server, device and client using smart pointers
            unique_ptr<Tcp::Server> server(new Tcp::Server);
            auto device = make_shared<Device:: Operation>();
            auto client = make_shared<Tcp::Client>();

            server->createServer(51111);
            int loop = true;
            while(loop)
            {
                try
                {
                    // listen and accept new client
                    // set to true for continous loop, set to false or no parameter for one time server use
                    server->Listen(true);

                    // received data from web client
                    string data = server->Read();
                    cout << "data from web client: " << data << endl;
                    // process received data
                    cout << "device status: " << device->processData(data) << endl;

                    // tcp client connecting to server
                    // provide remote endpoint port and ip
                    // if ip is not provided it will default to localhost
                    client->Connect(5555);
                    // send data to web client
                    client->Send(data);
                    // close client socket
                    client->Close();

                    cout << "data to web client: " << data << endl;
                    cout << "waiting for new data ... \n\n";

                    // close server socket
                    // since Listen(true) is set to continous loop, close operation will only close the newsockfd but not sockfd
                    server->Close();
                }
                catch (SocketError& e)
                {
                    loop = false;
                    cerr << "ctrl server error: " << e.what() << endl;
                    cout << "Please restart server." << endl;
                    server.reset(nullptr);
                    exit(1);
                }
            }
         server.reset(nullptr);
         exit(1);
        }

        /******************************************
    	* a simple tcp server using stack memmory *
        *******************************************/
        static void startCtrl2()
        {
            cout << "\n*** C++ IO-Control Project Using Stack Memory ***\n" << endl;

            int serverport = 51111;
            int clientport = 5555;
            Tcp::Server server = Tcp::Server(serverport);

            while(true)
            {
                try
                {
                    server.Listen(true);
                    string data = server.Read();
                    cout << "data from web client: " << data << endl;

                    auto device = make_shared<Device::Operation>();
                    string msg = device->processData(data);
                    cout << "device status: " << msg << endl;

                    auto client = make_shared<Tcp::Client>(clientport);
                    cout << "send data back to web client: " << client->Send(data) << endl;
                    client->Close();

                    cout << "waiting for new data ... \n\n";
                    server.Close();
                }
                catch (SocketError& e)
                {
                    cerr << "ctrl server error: " << e.what() << endl;
                }
            }
        }

};



