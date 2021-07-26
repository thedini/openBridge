import serial
import csv
import sys
USB_PORT = sys.argv[1]
ser = serial.Serial(USB_PORT, 9600)


with open("values.csv") as value_file:
    file_reader = csv.reader(value_file, delimeter=',')
    for line in file_reader:
        current_val = line[0]
        ser.write(current_val.encode())
        print(f"Value should be {current_val}")