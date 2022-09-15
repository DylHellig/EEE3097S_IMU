from secrets import token_bytes
from Crypto.Cipher import AES
import serial
import aes as e
import compression as c
import preprocessing as pre

original = "hi"

s = serial.Serial('/dev/ttyUSB2', 
    baudrate=9600, 
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    xonxoff=False
)
vals = []
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

print(key)
print(nonce)
print(tag)
print(cipher)

plain = e.decrypt(key, nonce, cipher, tag)
print(plain)

decomp = c.decompress(plain)
print(str(decomp)[2:-1])

# with open('nonce.txt', 'rb') as f:
#     nonce = f.read()
    
# with open('key.txt', 'rb') as f:
#     key = f.read()

# with open('tag.txt', 'rb') as f:
#     tag = f.read()



# plain = e.decrypt(key, nonce, data, tag)
# print(plain)

# decomp = c.decompress(plain)
# print(str(decomp)[2:-1])

