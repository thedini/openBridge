import serial

ser = serial.Serial('COM11', 9600)

while True:
    speed = input("Enter wind speed: ")
    ser.write(speed.encode())
    print(f"Speed should be {speed}")