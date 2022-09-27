import serial

s = serial.Serial('/dev/ttyUSB0', 
    baudrate=9600, 
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    xonxoff=False
)

entries = []
print('Reading Data From Serial Port...')
while True:
    try:
        data = s.readline()#.decode()
        entries.append(data)
        print(data)
    except:
        print("Keyboard Interrupt")
        break

with open('ciphertext.txt', 'wb') as f:
    for entry in entries:
        f.write(entry)
