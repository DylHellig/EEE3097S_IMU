import zlib
from Crypto.Cipher import AES
from secrets import token_bytes

key = token_bytes(16)

def compress(plaintext, level):
    comp = zlib.compress(plaintext.encode('utf-8'), level)
    return comp

def encrypt(msg):
    cipher = AES.new(key, AES.MODE_EAX)
    nonce = cipher.nonce
    ciphertext, tag = cipher.encrypt_and_digest(msg)
    return nonce, ciphertext, tag

if __name__ == '__main__':

    data = "hi"
    comp = compress(data, 8)
    nonce, cipher, tag = encrypt(comp)
    print(nonce)
    print(key)
    print(tag)
    with open('sample.txt', 'w') as f:
        f.write(str(cipher))

    with open('nonce.txt', 'w') as f:
        f.write(str(nonce))
    
    with open('key.txt', 'w') as f:
        f.write(str(key))

    with open('tag.txt', 'w') as f:
        f.write(str(tag))
