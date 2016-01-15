
/*
 * File:   Controller.h
 * Author: Ed Alegrid
 *
 */

#pragma once
#include <memory>
#include "TcpServer.h"
#include "TcpClient.h"
#include "Device.h"

namespace Controller {
using namespace std;

class App
{
    public:
        App() {}
        ~App() {}

        /* application logic goes here */
        void startCtrlAction()
        {
            cout << "\n*** C++ IO-Control Module Project ***\n" << endl;

            /* server socket port details for incoming web client control code */
            int serverport;
            serverport = 51111;

            // option for entering port no. during app startup
            /*cout << "Enter TCP server localhost port no: ";
            cin >>serverport;*/

            unique_ptr<TcpServer::ServerSocket> server(new TcpServer::ServerSocket);
            // create server and provide server port
            server->createServer(serverport);

            /* client socket details for web client websocket server */
            char* ip = "localhost";
            char* webclientport = "5555";

            int loop = true;
            while(loop)
            {
                try
                {

                    // listen and accept new client
                    server->Listen(true); // set to true for continous loop, set to false or no parameter for one time server use

                    // receive data from web client
                    const char* data = server->Read();
                    cout << "data from web client: " << data << endl;

                    // process received data
                    unique_ptr< Device:: ControlLogic> ControlModule(new  Device::ControlLogic);
                    const char* msg = ControlModule->processData(data);
                    cout << "device status: " << msg << endl;

                    // create client
                    unique_ptr<TcpClient::ClientSocket> client(new TcpClient::ClientSocket);
                    client->Connect(webclientport); // provide remote endpoint port and ip, if ip is not provided it will default to "localhost"
                    // send data to web client
                    client->Send(data);
                    client->Close();
                    cout << "send data to web client: " << data << endl;

                    cout << "waiting for new data ... \n" << endl;
                    // since Listen(true) is set to continous loop, close operation will only close the newsockfd but not sockfd
                    server->Close();

                }
                catch (SocketError& e)
                {
                    loop = false;
                    cerr << "server error: " << e.what() << endl;
                    cout << "Please restart server." << endl;
                    server.reset(nullptr);
                    exit(1);
                }
            }

         server.reset(nullptr);
         exit(1);

        }

};

}
