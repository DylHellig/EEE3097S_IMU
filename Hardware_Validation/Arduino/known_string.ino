#include <AES.h>
#include <Crypto.h>
#include <SPI.h>
#include <ICM_20948.h>
#include <stdint.h>
#include <ctype.h>
#include <Base64.h>
#include "shox96_0_2.h"
#include "unishox2.h"

//program constants
#define BUFFER_SIZE 1024
#define BLOCK_SIZE 16

//General GVs
volatile byte state = LOW;

char buff[BUFFER_SIZE] = "This is a known string";

char x[10];
bool got_key = false;
String k;

//compression GVs
char comp[BUFFER_SIZE];//compression data buffer

//Encryption GVs
AES128 aes128;
uint8_t key[17] = "\x05\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
char key_str[17];
char tmp[BLOCK_SIZE+1];
uint8_t cypher[BLOCK_SIZE+1];
char cystring[2048];//encryption data buffer
char ciphertext[BUFFER_SIZE];


//Interfaces
HardwareSerial Serial2(PA3, PA2);


void setup() {

  Serial2.println("Initializing Setup...");
  Serial2.begin(9600); //initialize serial connection
  aes128.setKey(key,16); //initialize encryption with key
  pinMode(PC9, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PA0), start_stop, RISING);//interrupt to start/stop streaming data
}

int num_readings = 1;// counter to choose number of IMU readings to store in buffer
void loop() {
  //button press to start sending data
  if(state){
    //check ICM ready to send data
      digitalWrite(PC9, HIGH);
      
      

      //Serial2.println(buff);

      empty(cystring);//empty ciphertext buffer
      encryption(buff);//encrypt IMU data

      //Serial2.println(cystring);
     
      empty(comp);// empty compression data buffer
      //compress ciphertext using Unishox2
      unishox2_compress_simple(cystring, strlen(cystring), comp);
      //Serial2.println(comp);
      //print compression data
      print_comp();
      delay(6000);//1 second delay
    
  }
  //button press stop sending data
  else{
    digitalWrite(PC9, LOW);    
  }
      
  
}//end loop

//function to print data needed for decompression on host machine
void print_comp(){
  Serial2.println("Data:");
  Serial2.println(strlen(cystring));
  Serial2.write(comp);
  Serial2.println();
}

//function to convert hex string to a byte array
void toBytes(const char* src, uint8_t* x, int start, int stop){
    int value;
    for(int i=0; i<stop; i++){
        sscanf(src+start, "%02x", &value);
        start += 2;
        x[i] = value;
    }
}

//interrupt function for button press
void start_stop(){
  state = !state;
}

//function to empty buffer contents
void empty(char * x){
  memset(x, 0, sizeof(x));
}


//function to load imu data into buffer
void loadBuffer(ICM_20948_SPI *sensor){
    
  float ax = sensor->accX();
  formattedString(ax, 3);
  sprintf(buff+strlen(buff), "%s,", x);
  
  float ay = sensor->accY();
  formattedString(ay, 3);
  sprintf(buff+strlen(buff), "%s,", x);
  
  float az = sensor->accZ();
  formattedString(az, 3);
  sprintf(buff+strlen(buff), "%s,", x);
  
  float gx = sensor->gyrX();
  formattedString(gx, 3);
  sprintf(buff+strlen(buff), "%s,", x);
  
  float gy = sensor->gyrY();
  formattedString(gy, 3);
  sprintf(buff+strlen(buff), "%s,", x);
  
  float gz = sensor->gyrZ();
  formattedString(gz, 3);
  sprintf(buff+strlen(buff), "%s,", x);

  float tmp = sensor->temp();
  formattedString(tmp, 3);
  sprintf(buff+strlen(buff), "%s", x);
  sprintf(buff+strlen(buff), "\n");
  
}

//fucntion to format buffer data to a string
void formattedString(float val, int decimals){
  dtostrf(val, -1, decimals, x); 
}

//encryption function
//splits larger data into 16 byte blocks to be encrypted
//joins 16 byte ciphertext together to get a single ciphertext string
void encryption(char * plaintext){
  
  int n = strlen(plaintext);
  int lo = 0;
  int hi = 16;
  bool str_ended = false;
  while(!str_ended){
    if((n-lo)<16){
      strncpy(tmp, plaintext+lo, n-lo);
      aes128.encryptBlock(cypher,(uint8_t*) tmp);

      for(int i=0; i<BLOCK_SIZE; i++){
        sprintf(cystring+strlen(cystring), "%02X", cypher[i]);
      }
      str_ended = true;
    }
    else{
      strncpy(tmp, plaintext+lo, hi);
      aes128.encryptBlock(cypher,(uint8_t*) tmp);

      for(int i=0; i<BLOCK_SIZE; i++){
        sprintf(cystring+strlen(cystring), "%02X", cypher[i]);
      }
    }
    lo += hi;
    memset(tmp, 0, sizeof(tmp));
  }
     
}


