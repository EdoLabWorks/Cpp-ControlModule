/*
* File:   main.ccp
* Author: Ed Alegrid
*
* Use any Linux C++11 compliant compiler or IDE. 
*
* 
* This is a very simple TCP client/server library with non-blocking read/send operations.
* It was mainly use for testing TCP socket communications with Node-WebControl project.
* 
*/

#include <iostream>
#include <memory>
#include "app/controller.h"

using namespace std;

int main()
{
    unique_ptr<App> app(new App);

    /* run and test each method one at a time */

    app->startCtrl();
    //app->startTest();
    //app->startOtherTest();
    //app->startEchoServer();

   return 0;
}
