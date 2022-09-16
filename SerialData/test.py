import serial

s = serial.Serial('/dev/ttyUSB2', 
    baudrate=9600, 
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    xonxoff=False
)
x = 1
while True:
    try:
        data = s.readline()
        if(x==1):
            print(data[:-3])
        else:
            print(data[1:-3])
        x += 1
    except:
        print("Keyboard Interrupt")
        break