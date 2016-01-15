
/*
 * File:   main.ccp
 * Author: Ed Alegrid
 *
 * For now, this app will work only on Linux OS.
 * Use any C++11 compliant compiler.
 * This is a single process synchronous blocking code.
 *
 */

#include <iostream>
#include <memory>
#include "Controller.h"

using namespace std;

int main()
{

    unique_ptr<Controller::App> app(new Controller::App);
    app->startCtrlAction();

   return 0;
}
