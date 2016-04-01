
/*
 * File:   main.ccp
 * Author: Ed Alegrid
 *
 * For now, this app will work only on Linux OS.
 * Use any C++11 compliant compiler.
 * This is a simple synchronous TCP client/server process with non-blocking read timeout.
 * Mainly use for testing TCP socket communication with Node-WebControl Project.
 * 
 */

#include <iostream>
#include <memory>
#include "Controller.h"

using namespace std;

int main()
{

    unique_ptr<Controller::App> app(new Controller::App);
    app->startCtrl();

   return 0;
}
