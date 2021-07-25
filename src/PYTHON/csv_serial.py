#!/bin/python3
# Created by Soups71

import serial
import csv
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
        file_reader = csv.reader(value_file, delimeter=',')
        for line in file_reader:
            current_val = line[0]
            # Encode the value to binary
            serial_bus.write(current_val.encode())
            print(f"Value should be {current_val}")
            sleep(5)

if __name__ == "__main__":
    main()