import serial

ser = serial.Serial('COM11', 9600)

while True:
    user_input = input("Enter current value: ")
    ser.write(user_input.encode())
    print(f"Value should be {user_input}")