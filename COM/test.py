import serial

ser = serial.Serial()
ser.baudrate = 57600
ser.port = 'COM7'
ser.open()

test1 = bytes(1)
test2 = bytes([20])
test3 = bytes(b'\xff')
print(test2)

while True:
    g = int(input())
    if g==1:
        print('g=1')
        ser.write(test1)
    elif g==2:
        ser.write(test2)
    elif g==3:
        ser.write(test3)