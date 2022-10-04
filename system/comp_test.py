import serial
import unishox2
import sys
import importlib
import chardet
#print(sys.getdefaultencoding())

s = serial.Serial('/dev/ttyUSB0', 
    baudrate=9600, 
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    xonxoff=False
)
num_entries = 10
count = 0
entries = []
print('Reading Data From Serial Port...')
while True:
    try:
        data = s.readline()
        entries.append(data)
        #print(data)
        if(count ==  num_entries):
            break
        if(data == b'Data:\r\n'):
            print(f"entry: {count}")
            count += 1
    except:
        print("Keyboard Interrupt")
        break

n = 0
comp = b''
#comp = ""
p = 0
f = 0
decompressed = []
for i, entry in enumerate(entries):
    if(entry == b'Data:\r\n'):
        try:
            n = int(entries[i+1])
            print(n)
            j = i+2
        
            while(entries[j] != b'Data:\r\n'):
                comp += entries[j]
                j += 1
            
            decomp = unishox2.decompress(comp, n)
            #print(decomp)
            # print(len(decomp))
            if(len(decomp) == n):
                decompressed.append(decomp)
                print(f"Pass: {len(comp)}")
                p += 1
                
            else:
                print(f"Fail: {len(comp)}")
                
                f += 1
            comp = b''
            # comp = ""
        except IndexError:
            pass
print(f"{(p/count)*100}% passed")

with open('decomp.txt', 'w') as f:
    for decomp in decompressed:
        f.write(f"{decomp}\n")
        