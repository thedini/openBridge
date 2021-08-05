"""
Created by: Soups71

Usage: python3 fake_multi_data.py OUTFILE COUNT

Count is the number of values for each device to be created
"""

from random import randint
import sys




def temp_data(file):
    temp = randint(32, 120)
    line = f"0,{temp}\n"
    file.write(line)
    #return line
def bat_data(file):
    voltage = randint(9, 15)
    current = randint(0, 5)
    temp = randint(50, 130)
    line = f"1,{voltage},{current},{temp}\n"
    file.write(line)
    #return line
def wind_data(file):
    speed = randint(0, 50)
    wind_dir = randint(0, 350)
    line = f"2,{speed},{wind_dir}\n"
    file.write(line)
    #return line
def humid_data(file):
    humidity = randint(1, 100)
    line = f"3,{humidity}\n"
    file.write(line)
    #return line

def depth_data(file):
    depth = randint(3, 1000)
    line = f"4,{depth}\n"
    file.write(line)
    #return line



def main():
    output_file = sys.argv[1]
    number_of_lines = sys.argv[2]
    if number_of_lines == None:
        exit(1)
    with open(output_file, "w+") as writer:
        for i in range(int(number_of_lines)):
            temp_data(writer)
            bat_data(writer)
            wind_data(writer)
            humid_data(writer)
            depth_data(writer)
    
    



if __name__ == "__main__":
    main()