#!/bin/python3
# Created by Soups71

import serial
import sys
from time import sleep

# Command line parameters
USB_PORT = str(sys.argv[1])
FILENAME = str(sys.argv[2])


def main():
    # Set up serial interface with baud rate of 9600
    serial_bus = serial.Serial(USB_PORT, 9600)

    # For each line in the csv write the value to the serial bus
    with open(FILENAME) as value_file:
        for line in value_file:
            # Encode the value to binary
            serial_bus.write(line.strip().encode())
            print(f"Values should be {line.strip()}")
            sleep(5)

if __name__ == "__main__":
    main()