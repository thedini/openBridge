/*
 * Created By: Soups
 * This program is meant to act as an example of the usage of the new functionality
 * documentation is currently being written for this project and the code base
 * is being tweaked daily.
 *
 * Same as the version 2 branch but I tried to add some threading
 *
 * Last Updated: 10/24/21
 */
#include<windows.h>
#include <vector>
#include<stdio.h>
#include "windows_Serial.h"
#include "N2kmessage.h"
#include <thread>
#include <mutex>
#include <Windows.h>

std::string convertToString(std::vector<std::string> &data);
bool add_message(std::vector<std::string> &data);
void send_msg(bool verbose=true);
void craft_msg();


std::mutex locker;
Serial serial_port = Serial("COM27");
std::vector<std::vector<std::string>> queued_messages;


int main()
{
    serial_port.setupParams(115200, 8, 0, 0);
    std::thread crafter(craft_msg);
    std::thread sender(send_msg, true);

    crafter.join();
    sender.join();

    serial_port.close_handle();
    return 0;
}


void craft_msg(){
    std::vector<std::string> indi_message;
    int wind_speed = 0;
    int times = 0;
    int i = 0;
    while ( i < 10) {
        //printf("%d\n", i);
        wind_speed = (wind_speed % 20) + 1;
        times = (times % 2) + 1;
        SetN2kWindSpeed(indi_message, 1, wind_speed, 20, 2);
        add_message(indi_message);
        indi_message.clear();
        SetN2kWaterDepth(indi_message, 0, wind_speed / 3.3, 0, 1000);
        add_message(indi_message);
        indi_message.clear();
        //send_msg(queued_messages, serial_port, 1);
        Sleep(1000);
        i++;

    }
}




std::string convertToString(std::vector<std::string> &data){
    std::string csv_string = "";
    for(auto x: data) {
        csv_string += x + ",";
    }
    csv_string += "=";
    return csv_string;
}
bool add_message(std::vector<std::string> &data){
    std::string uid = data.front();
    locker.lock();
    std::vector<std::vector<std::string>> current_vect = queued_messages;
    locker.unlock();
    int idx = 0;
    bool updated = false;
    for(auto x: current_vect) {
        if(x.front() == uid){
            //printf("Messege updated in queue\n");
            current_vect.at(idx) = data;
            updated = true;
        }
        idx++;
    }
    if(!updated) {
        //printf("New Messege added to queue\n");
        current_vect.push_back(data);
    }
    locker.lock();
    queued_messages = current_vect;
    locker.unlock();
    return updated;

}


void send_msg(bool verbose){

    for(int i = 0; i<100; i++) {

        std::string data_str;
        locker.lock();
        std::vector<std::vector<std::string>> raw_vec = queued_messages;
        locker.unlock();
        int vec_length = raw_vec.size();
        printf("%d\n", vec_length);
        for (int idx = 0; idx < vec_length; idx++) {

            data_str = convertToString(raw_vec[idx]);
            //printf("%s\n", data_str.c_str());
            if (verbose) {
                printf("%s\n", data_str.c_str());
            }
            serial_port.sendData(data_str);
            Sleep(100);
        }
    }
}