// -*- C++ -*-
/*
 * File:   Controller.h
 * Author: Ed Alegrid
 *
 */

#pragma once
#include "TcpServer.h"
#include "TcpClient.h"
#include "Device.h"

namespace Controller{

class App
{
    public:
        App() {}
        ~App() {}

        /*   All application logic goes here */
        int startCtrlAction()
        {
            cout << "\n*** C++ IO-Control Module Project ***\n" << endl;

            /* server socket port details for incoming web client control code */
            int serverport;
            serverport = 51111;

            // option on entering port no. during app startup
            /*cout << "Enter TCP server localhost port no: ";
            cin >>serverport;*/

            // create socket, bind it and start listening
            unique_ptr<TcpServer::ServerSocket> server(new TcpServer::ServerSocket());
            server->createSocket(serverport);

            /* client socket details for web client websocket server */
            char* ip = "localhost";
            char* clientport = "5555";

            int loop = true;
             while(loop)
             {
                try
                {
                    // listen and accept new client
                    server->acceptSocket();
                    // receive data from web client
                    const char* data = server->readData();
                    cout << "control data from web client: " << data << endl;
                    // process received data
                    unique_ptr< Device:: ControlLogic> ControlModule(new  Device::ControlLogic());
                    const char* msg = ControlModule->processData(data);
                    cout << "device status: " << msg << endl;
                    // send confirmation data to web client
                    unique_ptr<TcpClient::ClientSocket> client(new TcpClient::ClientSocket());
                    client->sendMsg(ip, clientport, data);
                    cout << "sending confirmation code to web client: " << data << endl;

                    cout << "waiting for client connection ... " << endl;
                }
                catch (exception& e)
                {
                    loop = false;
                    cerr << "server error: " << e.what() << endl;
                    cout << "Please restart server." << endl;
                    server.reset(nullptr);
                    exit(1);
                }
             }

        return 1;

        }
};

}
