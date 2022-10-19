from secrets import token_bytes
from Crypto.Cipher import AES
import serial
import aes as e
import compression as c
import preprocessing as pre

original = "hi"

s = serial.Serial('/dev/ttyUSB0', 
    baudrate=9600, 
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    xonxoff=False
)
vals = []
print('Reading Data From Serial Port...')
while True:
    try:
        data = s.readline()
        vals.append(data)
        print(data)
        if(data == b'\x000\n'):
            break 
    except:
        print("Keyboard Interrupt")
        break


key, nonce, tag, cipher, _ = vals

key = key[:-1]
nonce = nonce[1:-1]
tag = tag[1:-1]
cipher = cipher[1:-1]

plain = e.decrypt(key, nonce, cipher, tag)

decomp = c.decompress(plain)
print("Data after decryption and decompression:")
final = str(decomp)[2:-1]
print(final)

print("Checking correctness...")
if(final==original):
    print('The retrieved data is exactly the same as the original.')
else:
    print("data is not correct")


