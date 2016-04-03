/*
* File:   main.ccp
* Author: Ed Alegrid
*
* For now, this app will work only on Linux OS.
* Use any C++11 compliant compiler or IDE. I have used it with CodeBlocks and NetBeans w/o problems.
* I always set it to emit all compiler warnings and try to resolve each one by one.
* There are still plenty of warnings!
*
* 
* This is a very simple synchronous TCP client/server library with non-blocking read timeout.
* It was mainly use for testing TCP socket communications with Node-WebControl project.
* 
*/

#include <iostream>
#include <memory>
#include "Controller.h"

using namespace std;

int main()
{

    unique_ptr<Controller::App> app(new Controller::App);

    /* run and test each method one at a time */

    app->startCtrl();
    //app->startTest();
    //app->startOtherTest();
    //app->startEchoServer();

   return 0;
}
