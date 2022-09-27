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
        data = s.readline().decode()
        entries.append(data)
        print(data)
    except:
        print("Keyboard Interrupt")
        break

with open('imu_data.txt', 'w') as f:
    for entry in entries[1:]:
        f.write(entry)

with open('imu_data.csv', 'w') as f:
    for entry in entries[1:]:
        f.write(entry)