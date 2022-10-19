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

char buff[BUFFER_SIZE] = "450351BE48BE28262F86AF925F9928AF3A653";

char x[10];


//compression GVs
char comp[BUFFER_SIZE];//compression data buffer


//Interfaces
HardwareSerial Serial2(PA3, PA2);
SPIClass SPI_1(PA7, PA6, PA5);
ICM_20948_SPI myICM;

void setup() {

  Serial2.println("Initializing Setup...");
  Serial2.begin(9600); //initialize serial connection
  pinMode(PC9, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PA0), start_stop, RISING);//interrupt to start/stop streaming data
  SPI_1.begin(PC4);
  myICM.begin(PC4, SPI_1);
  bool initialized = false;
  //initializing IMU to ensure it is ready to stream data
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

int num_readings = 1;// counter to choose number of IMU readings to store in buffer
void loop() {
  //button press to start sending data
  if(state){
    //check ICM ready to send data
    if(myICM.dataReady()){
      digitalWrite(PC9, HIGH);
      
      empty(buff);//empty IMU data buffer
      //loop to load a number of IMU readings into the buffer
      for(int i=0; i<num_readings; i++){
        myICM.getAGMT();
        loadBuffer(&myICM);
      }

      //Serial2.println(buff);

      
      empty(comp);// empty compression data buffer
      //compress ciphertext using Unishox2
      unishox2_compress_simple(buff, strlen(buff), comp);
      //Serial2.println(comp);
      //print compression data
      print_comp();
      delay(6000);//1 second delay
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

//function to print data needed for decompression on host machine
void print_comp(){
  Serial2.println("Data:");
  Serial2.println(strlen(buff));
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



