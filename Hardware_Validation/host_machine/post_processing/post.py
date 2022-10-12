labels = "AccX(mg),AccY(mg),AccZ(mg),GyrX(dps),GyrY(dps),GyrZ(dps),Temp(C)"

with open("decrypted.txt", "r") as f:
    decrypted = f.readlines()

with open("data/IMU_data.txt", "w") as f:
    f.write(labels)
    for line in decrypted:
        f.write(f"{line}\n")

with open("data/IMU_data.csv", "w") as f:
    f.write(f"{labels}\n")
    for line in decrypted:
        f.write(line)
