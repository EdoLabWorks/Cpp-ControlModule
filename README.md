# C++ IO-Control Project

A quick console app for Node WebControl Project using a simple TCP socket library.

It was mainly use for testing TCP socket communications with Node-WebControl project for PC based control systems.
 
However, you can easily use it as a general purpose TCP networking interface for
simple lightweight low payload client/server applications. 

The socket for both client and server is set to non-blocking rendering all subsequent read/send operations as non-blocking. 
The read buffer size is fixed to 1024 bytes which you can easily adjust to meet your requirements.

The api has some inspirations from NodeJS networking module.

### Usage

Use any Linux C++11 compliant compiler or IDE to try it.

Download [NodeJS web control](https://github.com/EdoLabWorks/NodeJS-Web-Control-Project) app and run it as a separate process.

#### Echo Server Demo
~~~~
#include <memory>
#include "tcp/server.h"

int main()
{
    auto s = make_shared<Tcp::Server>(51111);
    for (;;)
    {
        s->Listen(true);
        s->Send(s->Read());
        s->Close();
    }
    return 0;
}
~~~~


### License
MIT

 
