// -*- C++ -*-
/*
 * File:   Device.h
 * Author: Ed Alegrid
 *
 */
#pragma once

namespace Device{

class ControlLogic
{
    public:
        ControlLogic() {}
        ~ControlLogic() {}

        // control code operation
        const char* processData(const char message[256])
        {
            string RxData = message;
            const char* data;
            // device1
            if (RxData == "ON1"){
                data = "Device1 is ON";
            }
            else if(RxData == "OFF1"){
                data = "Device1 is OFF";
            }
            // device2
            else if (RxData == "ON2"){
                data = "Device2 is ON";
            }
            else if(RxData == "OFF2"){
                data = "Device2 is OFF";
            }
            // device3
            else if (RxData == "ON3"){
                data = "Device3 is ON";
            }
            else if(RxData == "OFF3"){
                data = "Device3 is OFF";
            }
            // device4
            else if (RxData == "ON4"){
                  data = "Device4 is ON";
            }
            else if(RxData == "OFF4"){
                  data = "Device4 is OFF";
            }
            // device5
            else if (RxData == "ON5"){
                data = "Device5 is ON";
            }
            else if(RxData == "OFF5"){
                data = "Device5 is OFF";
            }
            // device6
            else if (RxData == "ON6"){
                  data = "Device6 is ON";
            }
            else if(RxData == "OFF6"){
                  data = "Device6 is OFF";
            }
            else
                  data = "code not recognized";

            return data;
        }

};

}
