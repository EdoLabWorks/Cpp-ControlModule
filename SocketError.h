/*
* File:   SocketEror.h
* Author: Ed Alegrid
*
*/

#pragma once
#include <errno.h>

class SocketError : public std::exception
{
     std::string erMsg = "null";
     char* ErrCode;

     public:
        // with no parameter, error message defaults to errno
        SocketError() : ErrCode(strerror(errno)) {}
        // enter your own descriptive error message
        SocketError(const char* msg) : erMsg(msg) {}
        virtual ~SocketError() {}
        virtual const char* what() const noexcept override
        {
            if(erMsg == "null") {
            return ErrCode;
            }
            else{
            return erMsg.c_str();
            }
        }
};

