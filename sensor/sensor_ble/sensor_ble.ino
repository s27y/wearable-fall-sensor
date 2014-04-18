#include "I2Cdev.h"//add neccesary headfiles
#include "MPU6050.h"//add neccesary headfiles
#include <Wire.h>
#include <Streaming.h>
#include <string.h>

//====the offset of gyro===========
#define Gx_offset  -1.50
#define Gy_offset  0
#define Gz_offset  0.80
//====the offset of accelerator===========
#define Ax_offset -0.07
#define Ay_offset 0.02
#define Az_offset 0.14
//====================
MPU6050 accelgyro;

#define REG_ADDR_RESULT         0x00
#define REG_ADDR_CONFIG         0x02

#define ADDR_ADC121             0x58

int16_t ax,ay,az;//original data;
int16_t gx,gy,gz;//original data;
float Ax,Ay,Az;//Unit g(9.8m/s^2)
float Gx,Gy,Gz;//Unit ��/s
float loudness;



//
float RwEst[3];  //Rw estimated from combining RwAcc and RwGyro
float RwAcc[3];  //projection of normalized gravitation force 
float RwGyro[3];        //Rw obtained from last estimated value and gyro movement
float Awz[3];           //angles between projection of R on XZ/YZ plane and Z axis (deg)  - arctan2
float thresholding= 2.5;
unsigned long interval; //total running time TODO? might nead to reset at some stage
unsigned long lastMicros; // perivous measurement interval
char firstSample;	  //marks first sample
float wGyro;
//
char buffer [1000];

int measurementMode = 0;

/*
 * Initialise the IMU
 */
void initialiseAccelerationAndGyro() {
   // Initializing IMU sensor
   accelgyro.initialize();
  
   // Testing connections with IMU
   Serial.println(accelgyro.testConnection() ? "IMU connection successful":"IMU connection failure");
}

/*
 * Initialise the loudness sensor
 */
void initialiseLoudnessSensor()
{
   Wire.beginTransmission(ADDR_ADC121);        // transmit to device
   Wire.write(REG_ADDR_CONFIG);                // Configuration Register
   Wire.write(0x20);
   Wire.endTransmission();  
}

/*
 * Update the acceleration and Orientation values
 */
void updateAccelerationAndGyro() {
   accelgyro.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);//get the gyro and accelarator   
   //==========accelerator================================
   Ax=ax/16384.00;//to get data of unit(g)
   Ay=ay/16384.00;//to get data of unit(g)
   Az=az/16384.00;//to get data of unit(g)
   //===============gyro================================
   Gx=gx/131.00;
   Gy=gy/131.00;
   Gz=gz/131.00;
}




void getEstimatedInclination()
{
  static int i,w;
  static float tmpf,tmpf2;  
  static unsigned long newMicros; //new timestamp
  static char signRzGyro;  
  
  newMicros = micros();       //save the time when sample is taken
  updateAccelerationAndGyro();\
  interval = newMicros - lastMicros;
  lastMicros = newMicros;  
  
  RwAcc[0] = Ax;
  RwAcc[1] = Ay;
  RwAcc[2] = Az;
  
  //normalize3DVector(RwAcc);
  
  if (firstSample){
    for(w=0;w<=2;w++) RwEst[w] = RwAcc[w];    //initialize with accelerometer readings
  }else{
    //evaluate RwGyro vector
    if(abs(RwEst[2]) < 0.1){
      //Rz is too small and because it is used as reference for computing Axz, Ayz it's error fluctuations will amplify leading to bad results
      //in this case skip the gyro data and just use previous estimate
      for(w=0;w<=2;w++) RwGyro[w] = RwEst[w];
    }else{
      //get angles between projection of R on ZX/ZY plane and Z axis, based on last RwEst
        tmpf = Gx/1000;                         //get current gyro rate in deg/ms
        tmpf *= interval / 1000.0f;                     //get angle change in deg
        Awz[0] = atan2(RwEst[0],RwEst[2]) * 180 / PI;   //get angle and convert to degrees        
        Awz[0] += tmpf; 
        
          tmpf = Gy/1000;                         //get current gyro rate in deg/ms
        tmpf *= interval / 1000.0f;                     //get angle change in deg
        Awz[1] = atan2(RwEst[1],RwEst[2]) * 180 / PI;   //get angle and convert to degrees        
        Awz[1] += tmpf; 
        
          tmpf = Gz/1000;                         //get current gyro rate in deg/ms
        tmpf *= interval / 1000.0f;                     //get angle change in deg
        Awz[2] = atan2(RwEst[2],RwEst[2]) * 180 / PI;   //get angle and convert to degrees        
        Awz[2] += tmpf; 
        
      
      //estimate sign of RzGyro by looking in what qudrant the angle Axz is, 
      //RzGyro is pozitive if  Axz in range -90 ..90 => cos(Awz) >= 0
      signRzGyro = ( cos(Awz[0] * PI / 180) >=0 ) ? 1 : -1;
      
      //reverse calculation of RwGyro from Awz angles, for formula deductions see  http://starlino.com/imu_guide.html
      for(w=0;w<=1;w++){
        RwGyro[w] = sin(Awz[w] * PI / 180);
        RwGyro[w] /= sqrt( 1 + squared(cos(Awz[w] * PI / 180)) * squared(tan(Awz[1-w] * PI / 180)) );
      }
      RwGyro[2] = signRzGyro * sqrt(1 - squared(RwGyro[0]) - squared(RwGyro[1]));
    }
    
    //combine Accelerometer and gyro readings
    for(w=0;w<=2;w++) RwEst[w] = (RwAcc[w] + wGyro* RwGyro[w]) / (1 + wGyro);

    //normalize3DVector(RwEst);  
  }
  
  firstSample = 0;
}

float squared(float x){
  return x*x;
}


/*
 * Update the loudness value
 */
void updateLoudness()
{
    int getData;
    Wire.beginTransmission(ADDR_ADC121);        // transmit to device
    Wire.write(REG_ADDR_RESULT);                // get reuslt
    Wire.endTransmission();

    Wire.requestFrom(ADDR_ADC121, 2);           // request 2byte from device
    delay(1);
    
    if(Wire.available()<=2)
    {
      getData = (Wire.read()&0x0f)<<8;
      getData |= Wire.read();
      loudness = getData;
    }
}

/*
 * Convert a double to a char array
 */
int dToBuffer(float value, char* buffer, int start)
{
  int origin = start;
  if (value >= 1000)
  {
    return -1;
  }
  
  int mode = 0; 
  long v = (long)(value * 1000LL);
  
  if (v < 0)
  {
    buffer[start++] = '-';
    v = abs(v);
  }
  
  int i = 6;
  while (i > 0)
  {
    long b = pow(10, i--);
    int _v = v/b;
    if (mode || _v > 0)
    { 
      buffer[start++] = (char)(_v+48);
      mode = 1;
    }
    v = v - _v*b;
    if (i == 2)
    {
      if (buffer[start-1] == '-')
      {
        buffer[start++] = '0';
      }
      else if (origin - start == 0)
      {
        buffer[start++] = '0';
      } 

      buffer[start++] = '.';
    }
  }
  
  buffer[start] = '\0';
  return start;
}

/*
 * Utility to copy arrays
 */
int copyOver(char origin[], char target[], unsigned int size, unsigned int start)
{
    int index = 0;
    while (index < size)
    {
        target[start + index] = origin[index];
        index++;
    }
    
    return start+index;
}

/*
 * Buffer the current sensor measurements
 */
int bufferSensorData(int start) 
{
  if (start < 0) 
  {
    return -1;  
  }
  if(start == 0)
  {
    memset(buffer, 0, 1000);
  }
  
  char text[] = { '#', 'M', 'E', 'A', 'S', 'U', 'R', 'E', 'M', 'E', 'N', 'T', '|' };

  start = copyOver(text, buffer, 13, start);
  start = dToBuffer(RwEst[0], buffer, start);
  buffer[start++] = '|';
  start = dToBuffer(RwEst[1], buffer, start);
  buffer[start++] = '|';
  start = dToBuffer(RwEst[2], buffer, start);
  buffer[start++] = '|';
  start = dToBuffer(Gx, buffer, start);
  buffer[start++] = '|';
  start = dToBuffer(Gy, buffer, start);
  buffer[start++] = '|';
  start = dToBuffer(Gz, buffer, start);
  buffer[start++] = '|';
  start = dToBuffer(loudness, buffer, start);
  buffer[start++] = '#';
  buffer[start] = '\0';
  
  return start;
}

/*
 * Transmit the buffer to the base
 */
boolean bleTransmitBuffer() 
{
  if (Serial1)
  {
    Serial1.write(buffer);
    Serial1.flush();
    return true;
  }
  else
  {
    return false;  
  }
}

/*
 * Initialise components and devices
 */
void setup() {
    // Console (remove when not used)
    Serial.begin(9600);
    
    // Connect to the BLE module
    Serial1.begin(38400);
   
    Wire.begin();
    
    initialiseAccelerationAndGyro();
    initialiseLoudnessSensor();
}

void loop() {
  int bufferCount = 0;
  int afterThresholding = 0;
  while (true) 
  {
    if (Serial1.available())
    {
      measurementMode = Serial1.read();
      Serial.println(measurementMode);
    }
    
    //If greater than 0 => take measurement
    if (measurementMode > 48)
    {
      getEstimatedInclination();
      updateLoudness();
      
      if(bufferCount<320)
      {
        Serial.print(bufferCount);
        Serial.print(" " );
        bufferCount = bufferSensorData(bufferCount+1);
      }else //Buffer full
      {
        Serial.println("buffer FULL");
        if(afterThresholding>0)
        {
          //previous is  over thresholding
          Serial.print("**AFTER");
          Serial.print(afterThresholding);
          Serial.println();
          afterThresholding--;
          bleTransmitBuffer();
        }
        bufferCount=0;
      }
      if(isOverThresholding())
      {
        if(bufferCount>0)
        {
          Serial.println("**OVER SEND**");
          bleTransmitBuffer();
          bufferCount=0;
          afterThresholding=2;
        }
        else if(bufferCount=0)
        {
          Serial.println("**OVER NO BUFFER**");
          afterThresholding=2;
        }
        
      }
      // If measurement mode is 2 or less, do not buffer
      if (measurementMode <= 50)
      {
       //
      }
      else
      {
        Serial.println(measurementMode);
        /*
          BUFFER MEASUREMENTS AND SEND WHEN BUFFER IS FULL
        */
        
        //change delay to whatever you need
        
      }
      delay(50);
    }
    
    // if measurement mode is 1 (49) => one measurement only
    if (measurementMode < 50)
    {
      measurementMode = 0;
    }
    
  }
  

  
}


boolean isOverThresholding()
{
  float currentValue = sqrt(squared(RwEst[0])+ squared(RwEst[1])+ squared(RwEst[2]));
  if(currentValue > thresholding)
  {
    return true;
  }
  return false;
}
