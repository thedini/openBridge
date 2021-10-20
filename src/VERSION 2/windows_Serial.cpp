//
// Created by Jacso on 9/29/2021.
//

#include "windows_Serial.h"
Serial::Serial(std::string com_port){

    std::string full_com_path = "\\\\.\\"+com_port;
    hComm = CreateFileA(full_com_path.c_str(),                //port name
                        GENERIC_READ | GENERIC_WRITE, //Read/Write
                        0,                            // No Sharing
                        NULL,                         // No Security
                        OPEN_EXISTING,// Open existing port only
                        0,            // Non Overlapped I/O
                        NULL);        // Null for Comm Devices

    if (hComm == INVALID_HANDLE_VALUE)
        printf("Error in opening serial port\n");
    else
        printf("opening serial port successful");
}
int Serial::setupParams(int baudRate, int byteSize, int stopBytes, int parity){
    ZeroMemory(&dcbSerialParams, sizeof(dcbSerialParams));
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hComm, &dcbSerialParams)) {
        //error getting state
        fprintf(stderr,"error getting state ... ");
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = byteSize;
    dcbSerialParams.StopBits = stopBytes;
    dcbSerialParams.Parity = parity;
    if (!SetCommState(hComm, &dcbSerialParams)) {
        //error setting state
        fprintf(stderr,"error setting state ...");
    }
    return 0;
}
int Serial::sendData(std::string data_string){
    const char *data_buff = data_string.c_str();
//    printf("%s\n", data_buff);
    DWORD BytesToWrite = data_string.length();
    DWORD BytesWritten;
    WriteFile(hComm, data_buff, BytesToWrite, &BytesWritten, NULL);
//    printf("%lu\n", sizeof(&data_buff));
//    printf("%lu\n", &BytesWritten);
    return 0;
}
int Serial::close_handle(){
    CloseHandle(hComm);
    return 0;
}