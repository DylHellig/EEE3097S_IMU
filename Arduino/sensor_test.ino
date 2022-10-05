#include <AES.h>
#include <Crypto.h>
#include <SPI.h>
#include <ICM_20948.h>
#include <stdint.h>
#include <ctype.h>
#include <Base64.h>
#include "shox96_0_2.h"
#include "unishox2.h"

#define BUFFER_SIZE 1024
#define BLOCK_SIZE 16

//General GVs
volatile byte state = LOW;
char buff[BUFFER_SIZE];
char x[10];
bool got_key = false;
String k;

//compression GVs
char comp[BUFFER_SIZE];
char decomp[BUFFER_SIZE];

//Encryption GVs
AES128 aes128;
uint8_t key[17] = "\x05\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
char key_str[17];
char tmp[BLOCK_SIZE+1];
uint8_t cypher[BLOCK_SIZE+1];
char cystring[2048];
char ciphertext[BUFFER_SIZE];


//Interfaces
HardwareSerial Serial2(PA3, PA2);
SPIClass SPI_1(PA7, PA6, PA5);
ICM_20948_SPI myICM;

void setup() {

  Serial2.println("Initializing Setup...");
  Serial2.begin(9600);
  aes128.setKey(key,16);
  pinMode(PC9, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PA0), start_stop, RISING);
  SPI_1.begin(PC4);
  myICM.begin(PC4, SPI_1);
  bool initialized = false;
  
  while(!initialized){
    
    myICM.begin(PC4, SPI_1);
    myICM.startupDefault();
    Serial2.println("System Ready. Press Blue Button to Start/Stop Sending Data.");
    
    if(myICM.status != ICM_20948_Stat_Ok){
      Serial2.println("Trying again...");
      delay(500);
    }
    else{
      initialized = true;
    }

    Serial2.println("AccX(mg),AccY(mg),AccZ(mg),GyrX(dps),GyrY(dps),GyrZ(dps),Temp(C)");
    
  }
  
}

int num_readings = 1;
void loop() {
  //button press to start sending data
  if(state){
    //check ICM ready to send data
    if(myICM.dataReady()){
      digitalWrite(PC9, HIGH);
      
      empty(buff);
     
      for(int i=0; i<num_readings; i++){
        myICM.getAGMT();
        loadBuffer(&myICM);
      }
      Serial2.println(buff);
      delay(500);
    }
    // if ICM not ready to send data
    else{
      Serial2.println("Waiting for data...");
      delay(500);
    }
    
  }
  //button press stop sending data
  else{
    digitalWrite(PC9, LOW);    
  }
      
  
}//end loop

void print_comp(){
  Serial2.println("Data:");
  Serial2.println(strlen(cystring));
  Serial2.write(comp);
  Serial2.println();
}

void comp_metrics(uint32_t t1, uint32_t t2){

}

void toBytes(const char* src, uint8_t* x, int start, int stop){
    int value;
    for(int i=0; i<stop; i++){
        sscanf(src+start, "%02x", &value);
        start += 2;
        x[i] = value;
    }
}

//interrupt function
void start_stop(){
  state = !state;
}

void empty(char * x){
  memset(x, 0, sizeof(x));
}


//function to load imu data into buffer
void loadBuffer(ICM_20948_SPI *sensor){
    
  // float ax = sensor->accX();
  // formattedString(ax, 3);
  // sprintf(buff+strlen(buff), "%s", x);
  
  // float ay = sensor->accY();
  // formattedString(ay, 3);
  // sprintf(buff+strlen(buff), "%s,", x);
  
  // float az = sensor->accZ();
  // formattedString(az, 3);
  // sprintf(buff+strlen(buff), "%s", x);
  
  float gx = sensor->gyrX();
  formattedString(gx, 3);
  sprintf(buff+strlen(buff), "%s,", x);
  
  float gy = sensor->gyrY();
  formattedString(gy, 3);
  sprintf(buff+strlen(buff), "%s,", x);
  
  float gz = sensor->gyrZ();
  formattedString(gz, 3);
  sprintf(buff+strlen(buff), "%s", x);

  // float tmp = sensor->temp();
  // formattedString(tmp, 3);
  // sprintf(buff+strlen(buff), "%s", x);
  //sprintf(buff+strlen(buff), "\n");
  
}


void formattedString(float val, int decimals){
  dtostrf(val, -1, decimals, x); 
}

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


