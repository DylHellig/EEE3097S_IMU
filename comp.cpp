#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include<iostream>
#include<fstream>
using namespace std;

char in[21] = "ff1a1f58b68f6f258c5d";
uint8_t data[20];
int main(){
    int value;
    for(int i=0;i<20; i++){
        sscanf(in+i*2, "%02x", &value);
        data[i] = value;
        char c = data[i]+'0';
        printf("%c\n", c);
    }
}