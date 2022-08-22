from Crypto.Cipher import AES
from secrets import token_bytes
import time

#generate key
key = token_bytes(16)

#encrypt function
def encrypt(msg):
    cipher = AES.new(key, AES.MODE_EAX)
    nonce = cipher.nonce
    ciphertext, tag = cipher.encrypt_and_digest(msg.encode('ascii'))
    return nonce, ciphertext, tag

#decrypt function
def decrypt(nonce, ciphertext, tag):
    cipher = AES.new(key, AES.MODE_EAX, nonce=nonce)
    plaintext = cipher.decrypt(ciphertext)
    try:
        cipher.verify(tag)
        return plaintext.decode('ascii')
    except:
        return False

#function to check if decrypted data is the same as the original data
def check_decryption(plaintext, original):
    if plaintext == original:
        print('Decryption Successful')
    else:
        print('Decryption Failed')

#timing function
def get_time():
    t = time.perf_counter()
    return t

def do_encryption(filenames, timing=False, generate_files=False):
    for file in filenames:
        #read text file as a string
        with open('csv_as_text/'+file) as f:
            original = f.read()
    
        start = get_time()
        #encrypt data
        nonce, ciphertext, tag = encrypt(original)
        stop = get_time()
        encrypt_time = round(stop-start, 4) 
        
        start = get_time()
        #decrypt data
        plaintext = decrypt(nonce, ciphertext, tag)
        stop = get_time()
        decrypt_time = round(stop-start, 4)

        total_time = encrypt_time+decrypt_time

        #generate textfiles
        if generate_files:
            with open('encrypted/encrypted_'+file+'.txt', 'w') as f:
                f.writelines(str(ciphertext))

            start = get_time()
            with open('decrypted/decrypted_'+file+'.txt', 'w') as f:
                f.writelines(str(plaintext))
            stop = get_time()
            write_time = round(stop-start, 4)
            total_time += write_time

        print(f'Checking Decryption for {file}')
        check_decryption(plaintext, original)

        if timing:
            print(f'Encrption Execution Time {encrypt_time} seconds')
            print(f'Decryption Execution Time {decrypt_time} seconds')
            if generate_files:
                print(f'Time to write to text file {write_time} seconds')
            print(f'Total Execution Time {total_time} seconds')

        print('-------------------------------------')

        

        


if __name__ == '__main__':
    
    filenames = ['Turntable1.txt', 'Turntable2.txt', 'WalkingAround.txt']
    do_encryption(filenames, timing=True)