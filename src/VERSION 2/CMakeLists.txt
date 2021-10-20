cmake_minimum_required(VERSION 3.20)
project(open_bridge)

set(CMAKE_CXX_STANDARD 17)

add_library(N2kmessage "N2kmessage.cpp" "N2kmessage.h")
add_library(windows_Serial "windows_Serial.cpp" "windows_Serial.h")
add_executable(open_bridge main.cpp)

target_link_libraries(open_bridge N2kmessage windows_Serial)