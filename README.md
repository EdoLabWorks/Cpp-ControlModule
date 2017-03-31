# C++ IO-Control Project

A quick console app for Node WebControl Project using a simple TCP socket library.

You can use the TCP socket library as a general purpose networking interface for any
simple lightweight low payload client/server applications. 

The socket for both client and server is set to non-blocking rendering all subsequent read/send operations as non-blocking. 
The read buffer size is fixed to 1024 bytes which you can easily adjust to meet your requirements.


### Usage

Use any Linux C++11 compliant compiler or IDE to try it.

Using GCC g++, create the object file.
~~~
$ g++ -Wall -std=c++11 -c main.cpp -o main.o
~~~
Then create the main executable. 
~~~
$ g++ -o main main.o -pthread
~~~
Run the application.
~~~
$ ./main
~~~

Download [NodeJS web control](https://github.com/EdoLabWorks/NodeJS-Web-Control-Project) app and run it as a separate process.

The two applications should communicate to each other using a simple TCP IPC (inter-process communication). 

### License
MIT

 
