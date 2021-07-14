import serial
import csv
ser = serial.Serial('COM11', 9600)


with open("values.csv") as value_file:
    file_reader = csv.reader(value_file, delimeter=',')
    for line in file_reader:
        current_val = line[0]
        ser.write(current_val.encode())
        print(f"Value should be {current_val}")