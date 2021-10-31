//
// Created by Jacso on 9/29/2021.
//

#ifndef OPEN_BRIDGE_SERIAL_H
#define OPEN_BRIDGE_SERIAL_H
#include <string>
#include<windows.h>
class Serial{
private:

public:
    HANDLE hComm;
    DCB dcbSerialParams;
    Serial(std::string com_port);
    int setupParams(int baudRate, int byteSize, int stopBytes, int parity);
    int sendData(std::string data_string);
    int close_handle();

};


#endif //OPEN_BRIDGE_SERIAL_H
