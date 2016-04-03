*
*/

#pragma once

namespace Device {
using namespace std;

class ControlLogic
{
    public:
        ControlLogic() {}
        ~ControlLogic() {}

        // control code operation
        const string processData(string m)
        {
            string data;

            // device1
            if (m == "ON1"){
                data = "Device1 is ON";
            }
            else if(m == "OFF1"){
                data = "Device1 is OFF";
            }
            // device2
            else if (m == "ON2"){
                data = "Device2 is ON";
            }
            else if(m == "OFF2"){
                data = "Device2 is OFF";
            }
            // device3
            else if (m == "ON3"){
                data = "Device3 is ON";
            }
            else if(m == "OFF3"){
                data = "Device3 is OFF";
            }
            // device4
            else if (m == "ON4"){
                  data = "Device4 is ON";
            }
            else if(m == "OFF4"){
                  data = "Device4 is OFF";
            }
            // device5
            else if (m == "ON5"){
                data = "Device5 is ON";
            }
            else if(m == "OFF5"){
                data = "Device5 is OFF";
            }
            // device6
            else if (m == "ON6"){
                  data = "Device6 is ON";
            }
            else if(m == "OFF6"){
                  data = "Device6 is OFF";
            }
            else
                  data = "code not recognized";

            return data;
        }
};

}

