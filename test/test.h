
/*
* File:   test.h
* Author: Ed Alegrid
*
*/

#pragma once
#include <iostream>
#include <memory>
#include "../tcp/server.h"
#include "../tcp/client.h"


using namespace std;
class Test
{
    public:

        /*************************************************************************
    	* a quick client/server communication and non-blocking read timeout test *
        **************************************************************************/
        static void Test1()
        {
            cout << "\n*** C++ Test using dynamic memory allocation ***\n" << endl;

            try
            {
               	unique_ptr<Tcp::Server> server(new Tcp::Server(51111));
            	auto client = make_shared<Tcp::Client>(51111);
            	server->Listen();

            	// client read should timeout since server didn't send yet any msg
            	// the process should continue on the next operation
                client->Read(); //non-blocking client read timeout

                client->Send("one");
                cout << "client: " << server->Read() << endl;
                server->Send("two");
                cout << "server: " << client->Read() << endl;
                client->Send("three");
                cout << "client: " << server->Read() << endl;
                server->Send("four");
                cout << "server: " << client->Read() << endl;

                // server read should timeout since client didn't send data after the first one
                server->Read(); //non-blocking server read timeout

                client->Close();
                // since Listen() is set to one time use w/o parameter
                // close operation will close both newsockfd and sockfd and stop the process
                server->Close();
                server.reset(nullptr);
                client.reset();
            }
            catch (SocketError& e)
            {
                cerr << "error: " << e.what() << endl;
            }
        }

        /**************************
    	* test using stack memory *
        ***************************/
        static void Test2()
        {
            cout << "\n\n*** C++ Test using stack memory allocation ***\n" << endl;
            using namespace Tcp;

            try
            {
                Server s = Server(51111);
                Client c = Client(51111);

                s.Listen();
                c.Send("one");
                cout << "client: " << s.Read() << endl;
                s.Send("two");
                cout << "server: " << c.Read() << endl;
                c.Send("three");
                cout << "client: " << s.Read() << endl;
                s.Send("four");
                cout << "server: " << c.Read() << endl;

                c.Close();
                s.Close();

            }
            catch (SocketError& e)
            {
                cerr << "error: " << e.what() << endl;
            }
        }

        /****************************
    	* a simple echo server demo *
        *****************************/
        static void EchoServer()
        {
            cout << "\n\n*** C++ Echo Server Demo ***\n" << endl;

            auto s = make_shared<Tcp::Server>(51111);
            for (;;)
            {

                s->Listen(true);
                cout << "data received and sending it back: ";
                cout <<  s->Send(s->Read()) << endl;
                s->Close();
            }
            s.reset();
        }

        /* run all tests */
        static void startAll(){
            Test1();
            Test2();
            EchoServer();
        }

};

