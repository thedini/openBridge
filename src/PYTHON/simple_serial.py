#!/bin/python3
# Created by Soups71

import serial
import sys

# Get command line arguments
USB_PORT = str(sys.argv[1])

serial_bus = serial.Serial(USB_PORT, 9600)

while True:
    user_input = input("Enter current wind speed value: ")
    # Convert the value to binary
    serial_bus.write(user_input.encode())
    print(f"Value should be {user_input}")