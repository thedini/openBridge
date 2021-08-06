## Introduction

The various python scripts allow a computer to interface with the teensy.

## Usage

`pip3 install -r requirements.txt`

### Option 1 (Manual control):
* `sudo python3 simple_serial.py COM11`
    * This option allows for the real time input of wind speed and direction values when paired with the working_wind_input firmware in the ARDUINO source folder.

### Option 2 (Preloaded Values):

* `sudo python3 csv_serial.py COM11 sample.csv`
    * This option takes pre-loaded values from the csv and then sends them to the serial bus.
    * This can be paired with the multi_sender or working_wind_input firmware 
    * Example CSVs are provided for each
* MULTI_SENDER 
    * CSV format:
        * Column 1: Device type(0: Temp; 1: Battery; 2: Wind; 3: Humidity; 4: Depth)
        * Each column that follows is specific to the device type
        * Temp:
            * Column 2: Temperature
        * Battery:
            * Column 2: Voltage
            * Column 3: Current
            * Column 4: Battery Temperature
        * Wind:
            * Column 2: Speed
            * Column 3: Direction
        * Humidity:
            * Column 2: Humidity
        * Depth:
            * COlumn 2: Depth in feet
* WORKING_WIND_INPUT
    * CSV format:
        * Column 1: Wind speed
        * Column 2: Wind direction

## Extra Scripts and Resources

* There are 2 other scripts and 2 csv files also provided
* `python3 fake_multi_data.py OUTFILE COUNT`
    * Creates fake data for the multi_sender firmware.
    * Outputs data to csv
* `python3 fake_wind_data.py OUTFILE COUNT`
    * Creates fake data for the working_wind_input firmware
    * Outputs data to csv
* CSV files
    * These are example files that were created as examples. I highly suggest looking for real data sources to stream to the teensy or create a new csv of random values using the scripts proviced.


## Explanation

The parameters passed must include the USB connection that the teensy is connected too.
