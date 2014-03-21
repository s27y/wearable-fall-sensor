#include "I2Cdev.h"//add neccesary headfiles
#include "MPU6050.h"//add neccesary headfiles
#include <Wire.h>
#include <Streaming.h>
#include <string.h>
#include <math.h>
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

#define PI 3.14159265358979f


int16_t ax,ay,az;//original data;
int16_t gx,gy,gz;//original data;
float Ax,Ay,Az;//Unit g(9.8m/s^2)
float Gx,Gy,Gz;//Unit ��/s
float loudness;

float accel_angle_x,  
      accel_angle_y,  
      accel_angle_z; // in radian
      
float accel_angle_degx,  
      accel_angle_degy,  
      accel_angle_degz; // in degree
      
      
float RwEst[3];  //Rw estimated from combining RwAcc and RwGyro
float RwAcc[3];  //projection of normalized gravitation force 
float RwGyro[3];        //Rw obtained from last estimated value and gyro movement
float Awz[3];           //angles between projection of R on XZ/YZ plane and Z axis (deg)

unsigned long interval;
unsigned long lastMicros;
char firstSample;	  //marks first sample

unsigned long NumOfData = 0;

float wGyro;







//=====================Test Buffer========================
const unsigned int MAX_INPUT = 50;
void process_data (const char * data)
  {
  // for now just display it
  // (but you could compare it to some value, convert to an integer, etc.)
  Serial.println ("##########################");
  }  // end of process_data
void processIncomingByte (const byte inByte)
  {
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte)
    {

    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte
      
      // terminator reached! process input_line here ...
      process_data (input_line);
      
      // reset buffer for next time
      input_pos = 0;  
      break;

    case '\r':   // discard carriage return
      break;

    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;

    }  // end of switch
   
  } // end of processIncomingByte
//=============================================



//=======LOOP========
void loop() {
   updateAccelerationAndGyro();
   //calcXyzAngles();
   //printData();
   //updateLoudness();
   //bleTransmitSensorData();
   //delay(1000);
   //interval+=1000;
   newLoop();

}

void newLoop()
{
  getEstimatedInclination();
  //!!! Please note that printing more data will increase interval between samples. Try to keep it under 10ms (10,0000 us)   
  //printSerialTestData();
  printInterval();
  printGyroDegree();
  NumOfData++;
  if(NumOfData==10)
  {
    NumOfData=0;
    Serial.println("");
  }
}

void setup() {
   // Console (remove when not used)
   Serial.begin(9600);
   // Connect to the BLE module
   Serial1.begin(38400);
   Wire.begin();
   initialiseAccelerationAndGyro();
   initialiseLoudnessSensor();
   firstSample = 1;
   wGyro = 10;
}
//=======LOOP========
void initialiseAccelerationAndGyro() {
   // Initializing IMU sensor
   accelgyro.initialize();
  
   // Testing connections with IMU
   Serial.println(accelgyro.testConnection() ? "IMU connection successful":"IMU connection failure");
}

void initialiseLoudnessSensor()
{
   Wire.beginTransmission(ADDR_ADC121);        // transmit to device
   Wire.write(REG_ADDR_CONFIG);                // Configuration Register
   Wire.write(0x20);
   Wire.endTransmission();  
}

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

void bleTransmitSensorData() {
   char out[100];
   
   // xxxx.xxx|xxxx.xxx|xxxx.xxx|xxxx.xxx|xxxx.xxx|xxxx.xxx|xxxx.xxx"%4.3f|%4.3f|%4.3f|%4.3f|%4.3f|%4.3f|%4.3f"
   //sprintf(out, "%s", Ax);
   //Serial.println(out);
   printAccelerationAndGyro();
   
   if (Serial1.available()) {
      Serial1.write(out);
   }
}
void printAccelerationAndGyro()
{
  Serial.println("Value of Acceleration X Y Z: ");
  Serial.print(Ax);
  Serial.print(" , ");
  Serial.print(Ay);
  Serial.print(" , ");
  Serial.println(Az);
  Serial.println("Value of Gyro X Y Z: ");
  Serial.print(Gx);
  Serial.print(" , ");
  Serial.print(Gy);
  Serial.print(" , ");
  Serial.println(Gz);
}

void calcXyzAngles(){
   // Using x y and z from accelerometer
   // calculate x y and z angles
   float x_val, y_val, z_val, result;
   float x2, y2, z2; //power of x y z

   // Lets get the deviations from our baseline
   //x_val = (float)accel_value_x-(float)accel_center_x;
   //y_val = (float)accel_value_y-(float)accel_center_y;
   //z_val = (float)accel_value_z-(float)accel_center_z;

  x_val= Ax;
  y_val = Ay;
  z_val = Az;

   // Work out the squares 
   x2 = (x_val*x_val);
   y2 = (y_val*y_val);
   z2 = (z_val*z_val);

   //X Axis
   result=sqrt(y2+z2);
   result=x_val/result;
   accel_angle_x = atan(result);
   accel_angle_degx = accel_angle_x * RAD_TO_DEG; 
  
   //Y Axis
   result=sqrt(x2+z2);
   result=y_val/result;
   accel_angle_y = atan(result);
   accel_angle_degy = accel_angle_y * RAD_TO_DEG; 
   
   //Z Axis
   result=sqrt(x2+y2);
   result=z_val/result;
   accel_angle_z = atan(result);
   accel_angle_degz = accel_angle_z * RAD_TO_DEG; 
   
   float accel_virtual_x = cos(accel_angle_x) * Ax;
   float accel_virtual_y = cos(accel_angle_y) * Ay;
   float accel_virtual_z = sin(accel_angle_z) * Az
                          +sin(accel_angle_x) * Ax
                          +sin(accel_angle_y) * Ay;
}

void printData()
{
  Serial.print(interval);
  Serial.print(",");
  Serial.print(accel_angle_degx);
  Serial.print(",");
  Serial.print(accel_angle_degy);
  Serial.print(",");
  Serial.print(accel_angle_degz);
  Serial.println();

}


void printInterval()
{
  Serial.print(interval);
  Serial.print(",");
}
void printAccData()
{
  Serial.print(RwAcc[0]);  //Inclination X axis (as measured by accelerometer)
  Serial.print(",");
  Serial.print(RwEst[0]);  //Inclination X axis (estimated / filtered)
  Serial.print(",");    
  Serial.print(RwAcc[1]);  //Inclination Y axis (as measured by accelerometer)
  Serial.print(",");
  Serial.print(RwEst[1]);  //Inclination Y axis (estimated / filtered)
  Serial.print(",");    
  Serial.print(RwAcc[2]);  //Inclination Z axis (as measured by accelerometer)
  Serial.print(",");
  Serial.print(RwEst[2]);  //Inclination Z axis (estimated / filtered)  
  Serial.println("");
}
void printGyroDegree()
{
  Serial.print(RwGyro[0]);  //Inclination X axis (as measured by accelerometer)
  Serial.print(",");
  Serial.print(RwGyro[1]);  //Inclination X axis (estimated / filtered)
  Serial.print(",");    
  Serial.print(RwGyro[2]);  //Inclination Y axis (as measured by accelerometer)
  Serial.print(";");
  //
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

void normalize3DVector(float* vector){
  static float R;  
  R = sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
  vector[0] /= R;
  vector[1] /= R;  
  vector[2] /= R;   
}

float squared(float x){
  return x*x;
}
