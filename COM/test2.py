import serial

ser = serial.Serial()
ser.baudrate = 57600
ser.port = 'COM7'
ser.open()

data3 = []

while True:
    data = ser.read()
    #data2 = data.split(b'\\')
    data3.append(data)
    if data3:
        print(data3)
        data3 = []

print(data3)