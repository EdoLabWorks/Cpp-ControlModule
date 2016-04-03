# C++ IO-Control Project

A simple and quick console app and TCP socket library to communicate with Node WebControl Project using IPC.

For now, this app will only work on Linux OS.

### Usage
Use any C++11 compliant compiler or IDE. I have used it with CodeBlocks and NetBeans w/o problems.
I always set it to emit all compiler warnings and try to resolve them one by one.
There are still plenty of warnings!
 
This is a very simple synchronous TCP client/server library with non-blocking read timeout.
It was mainly use for testing TCP socket communications with Node-WebControl project for PC based control systems.
 
However, you can easily use it as a general purpose TCP networking interface for
simple lightweight intermittent low payload client/server applications. 

The read buffer size is fixed to 1024 which you can easily tweak to meet your requirements.
The api abstractions has some inspirations from NodeJS networking api.

The api is very simple and still close to its low-level classic UNIX domain socket based on C.  
Using C++, all data types are string based and creating a server and client instance is straightforward.
 

Download [NodeJS web control](https://github.com/EdoLabWorks/NodeJS-Web-Control-Project) app and run it as a separate process.

### License
MIT

 
