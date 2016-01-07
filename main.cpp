// -*- C++ -*-
/*
 * File:   main.ccp
 * Author: Ed Alegrid
 *
 * Use at least a C++11 compliant compiler
 * For now, this app will only work on Linux OS.
 *
 */

#include <iostream>
#include <memory>
#include "Controller.h"

using namespace std;

int main()
{
    unique_ptr<Controller::App> app(new Controller::App());
    app->startCtrlAction();

   return 0;
}
