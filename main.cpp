/*
* File:   main.ccp
* Author: Ed Alegrid
*
* A simple TCP client/server library with non-blocking read/send operations.
* It was mainly use for testing TCP socket communications with Node-WebControl project.
* 
* Use any Linux C++11 compliant compiler or IDE. 
*
*/

#include <iostream>
#include <memory>
#include "app/controller.h"

using namespace std;

int main()
{
    unique_ptr<App> app(new App);

    app->startCtrl();
    //app->startTest();
    //app->startOtherTest();
    //app->startEchoServer();

   return 0;
}
