## Introduction

The various python programs in order to use a computer to interface with the teensy's.

## Usage

`sudo pip3 install -r requirements.txt`

### Option 1 (Manual control):
* `sudo python3 simple_serial.py COM11`
    * This option allows for the real time input of wind speed values.

### Option 2 (Preloaded Values):

* `sudo python3 csv_serial.py COM11 sample.csv`
    * This option takes pre-loaded values from the csv and then sends them to the serial bus.

## Explanation

When the `working_wind_input.ino` example is loaded on the teensy, these python examples will modify the wind speed value on the displays.

