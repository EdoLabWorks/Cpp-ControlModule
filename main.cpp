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
#include "app/CppModule.h"
#include "test/test.h"

using namespace std;

int main()
{
   App::startCtrl();
   //Test::startAll();
   return 0;
}
