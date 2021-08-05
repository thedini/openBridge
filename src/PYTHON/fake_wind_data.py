"""
Created by: Soups71

Usage: python3 fake_wind_data.py OUTFILE COUNT
"""

from random import randint
import sys
def main():
    output_file = sys.argv[1]
    number_of_lines = sys.argv[2]
    if number_of_lines == None:
        exit(1)
    with open(output_file, 'w') as file_writer:
        for i in range(int(number_of_lines)):
            wind_speed = randint(0, 50)
            wind_dir = randint(0, 360)
            file_writer.write(f"{wind_speed},{wind_dir}\n")



if __name__ == "__main__":
    main()