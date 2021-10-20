#!/bin/python3
# Created by Soups71

import serial
import sys

def main():
    # Get command line arguments
    USB_PORT = str(sys.argv[1])

    serial_bus = serial.Serial(USB_PORT, 9600)

    while True:
        speed_input = input("Enter current wind speed value: ")
        dir_input = input("Enter current wind direction value: ")
        user_input = f"{speed_input},{dir_input}"
        # Convert the value to binary
        serial_bus.write(user_input.encode())
        print(f"Value should be {user_input}")

if __name__ == "__main__":
    main()