#include "AES.h"
#include "AES128.cpp"
#include "AESCommon.cpp"
#include "BlockCipher.cpp"
#include "BlockCipher.h"
#include "Crypto.h"
#include "Crypto.cpp"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include<iostream>
#include<fstream>
using namespace std;

#define FILE_SIZE 26

AES128 aes;
uint8_t key[17]= "\x05\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
char ciphertext[FILE_SIZE][256];
uint8_t decrypted[256];
uint8_t cipher[16];

char output[256];

void toBytes(const char* src, uint8_t* x, int start){
    int value;
    for(int i=0; i<16; i++){
        sscanf(src+start, "%02x", &value);
        start += 2;
        x[i] = value;
    }

}

void decrypt(const char* data){
    int n = strlen(data);
    int pos = 0;
    while(pos<=(n-2)){
       toBytes(data, cipher, pos);
        aes.decryptBlock(decrypted, cipher);
        sprintf(output+strlen(output), "%s", decrypted);
        pos += 32;
    }
}

int main(){
    aes.setKey(key, 16);

    ifstream file("ciphertext.txt");

    if(!file.is_open()){
        cout<<"Error opening file";
    }
    

    for(int i=0; i<FILE_SIZE; i++){
        file >> ciphertext[i];
    }

    for(int j=0; j<FILE_SIZE; j++){
        decrypt(ciphertext[j]);
        printf("%d: %s\n", j+1, output);
        memset(output, 0, sizeof(output));
    }

    

    return 0;
}

