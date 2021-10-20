/*
 * Created By: Soups
 * This program is meant to act as an example of the usage of the new functionality
 * documentation is currently being written for this project and the code base
 * is being tweaked daily.
 *
 * Last Updated: 10/20/21
 */
#include<windows.h>
#include <vector>
#include<stdio.h>
#include "windows_Serial.h"
#include "N2kmessage.h"
#include <Windows.h>

std::string convertToString(std::vector<std::string> &data);
int main()
{
    char vector_data[300];
    int wind_speed = 0;
    int times = 0;
    int i = 0;

    // Initialize the serial object
    Serial serial_port = Serial("COM4");
    serial_port.setupParams(115200, 8, 0, 0);

    while ( i < 100000) {
        wind_speed = (wind_speed % 20) + 1;
        times = (times % 2) + 1;
        std::vector<std::string> message;
        SetN2kWindSpeed(message, 1, wind_speed, 20, 2);
        std::string csv_data = convertToString(message);
        printf("%s\n", csv_data.c_str());
        serial_port.sendData(csv_data);
        Sleep(100);
        std::vector<std::string> message2;
        SetN2kWaterDepth(message2, 0, wind_speed / 3.3, 0, 1000);
        std::string csv_data2 = convertToString(message2);
        printf("%s\n", csv_data2.c_str());
        serial_port.sendData(csv_data2);
        Sleep(1000);
        i++;
    }
    serial_port.close_handle();
    return 0;
}

std::string convertToString(std::vector<std::string> &data){
    std::string csv_string = "";
    for(auto x: data) {
        csv_string += x + ",";
    }
    csv_string += "=";
    return csv_string;
}