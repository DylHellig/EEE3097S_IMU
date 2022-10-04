from copyreg import pickle
import serial
import random
import pickle as p
import time

#users = {"Dylan":"FCBF7570EA6BBB919FBE0188919E0FF3"}

s = serial.Serial('/dev/ttyUSB0', 
    baudrate=9600, 
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    xonxoff=False
)

def generate_key():
    hex_list = '0123456789ABCDEF'
    key = ''.join((random.choice(hex_list) for i in range(32)))
    return key
    

if __name__ == "__main__":
    key = "050102030405060708090A0B0C0D0E0F"
    #s.write(key.encode())

    while True:
        try:
            #s.write(key.encode())
            #time.wait(1)
            data = s.readline()#.decode()
            print(data)
        except:
            print("Keyboard Interrupt")
            break
    # with open("Users.pkl", "rb") as f:
    #     users = p.load(f)
    
    # with open("Users.pkl", "wb") as f:
    #     p.dump(users, f)
    # print("Welcome to the IMU Data Collection Service.\n")
    # print("Please Enter Your Username Below:")
    # username = input()

    # if username in users:
    #     print(f"Welcome Back {username}")
    #     with open("key.txt", "w") as f:
    #         f.write(users[username])
    # else:
    #     key = generate_key()
    #     users[username] = key
    #     print(f"Welcome {username} your decryption key is: {key}")
    #     print("Your key has been stored in the file: key.txt")
        
    #     with open("key.txt", "w") as f:
    #         f.write(users[username])
        
    #     with open("Users.pkl", "wb") as f:
    #         p.dump(users, f)

    #print(f"Hi {username} your decryption key can be found in the file key.txt")

