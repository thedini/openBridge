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
int add_message(std::vector<std::string> &data, std::vector<std::vector<std::string>> &queued_messages);
void send_msg(std::vector<std::vector<std::string>> &queued_messages, Serial &serial_interface, bool verbose=true);
int main()
{
    int wind_speed = 0;
    int times = 0;
    int i = 0;
    std::vector<std::vector<std::string>> queued_messages;
    Serial serial_port = Serial("COM4");
    serial_port.setupParams(115200, 8, 0, 0);
    std::vector<std::string> indi_message;
    while ( i < 100000) {
        wind_speed = (wind_speed % 20) + 1;
        times = (times % 2) + 1;
        SetN2kWindSpeed(indi_message, 1, wind_speed, 20, 2);
        add_message(indi_message, queued_messages);
        indi_message.clear();
        SetN2kWaterDepth(indi_message, 0, wind_speed / 3.3, 0, 1000);
        add_message(indi_message, queued_messages);
        indi_message.clear();
        send_msg(queued_messages, serial_port, 1);
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
int add_message(std::vector<std::string> &data, std::vector<std::vector<std::string>> &queued_messages){
    std::string uid = data.front();
    int idx = 0;
    for(auto x: queued_messages) {
        if(x.front() == uid){
            queued_messages.at(idx) = data;
            return 1;
        }
        idx++;
    }
    queued_messages.push_back(data);
    return 0;

}
void send_msg(std::vector<std::vector<std::string>> &queued_messages, Serial &serial_interface, bool verbose){
    std::string data_str;
    for(auto x: queued_messages) {
        data_str = convertToString(x);
        if(verbose){
            printf("%s\n", data_str.c_str());
        }
        serial_interface.sendData(data_str);

        Sleep(100);
    }
}