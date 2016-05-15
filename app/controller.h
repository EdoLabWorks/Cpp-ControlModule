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

        /********************
    	* webcontrol server *
        *********************/
        static void startCtrl()
        {
            cout << "\n*** C++ IO-Control Project ***\n" << endl;

            int serverport = 51111;
            int clientport = 5555;

            while(true)
            {
                try
                {

                    Tcp::Server server = Tcp::Server(serverport);
                    server.Listen();
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



