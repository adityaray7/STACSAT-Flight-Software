

//https://randomnerdtutorials.com/esp32-bmp388-arduino/
//https://randomnerdtutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/



#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <ESP32Servo.h>


#define BMP_SCK 18
#define BMP_MISO 19
#define BMP_MOSI 23
#define BMP_CS 5
#define SEALEVELPRESSURE_HPA (891.5) //1013.25
Adafruit_MPU6050 mpu;
Adafruit_BMP3XX bmp;




unsigned long int pkt = 0;
float current_level,maximum_altitude,previous_level,delta,alt,cal_alt,cal_x,cal_y;
int x,m_time,release_flag = 0;
char ss;

char mde = 'F';
char hs_deployed = 'N';
char pc_deployed = 'N';
char mast_raised = 'N';




//---------buzzer---------
int freq = 2000;
int channel = 0;
int resolution = 8;

//-------------------servo--------

Servo myservo;  // create servo object to control a servo
int servoPin = 13;


void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

 //-------------------initializing bmp and mpu--------------

  Serial.println("Adafruit BMP388 / BMP390 test");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  if (!bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  Serial.println("MPU6050 and bmp388 Found!");


//-------------------------mpu----------------------------------
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); //2,4,8,16
  Serial.print("Accelerometer range set to: 8");
  
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: 500 ");  //250,500,1000,2000

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ); //260,184,94,44,21,10,5
  Serial.print("Filter bandwidth set to: 5 Hz ");

  Serial.println("");
  delay(100);

//---------------------bmp388----------------------

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  //---------------buzzer---------------------
//
//  ledcSetup(channel, freq, resolution);
//  ledcAttachPin(14, channel);
//  ledcWriteTone(channel, 0);
//  ledcWrite(channel, 0);

  //------------------------servo--------------------

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);  
  myservo.attach(servoPin, 500, 2400); 

  cal_alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
}

void loop() {
  pkt++;

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  if( (Serial.available()== 0)){
  
  int x = Serial.parseInt();
    
  if (x == 84)
    {
      
      Serial.println("---------------------Calibrating--------------------");
      cal_x = g.gyro.x;
      cal_y = g.gyro.y;
      cal_alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
      Serial.print("Calibrated Altitude:");
      Serial.println(cal_alt);
      Serial.print("Calibrated x gyro:");
      Serial.println(cal_x);
      Serial.print("Calibrated x gyro:");

      Serial.println(cal_y);
      servomove();
      Serial.println("--------------Calibration Done---------------------");
      
      delay(5000);
    }
  }

 
  
  alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  
  alti();
  software_state();
  telemetry();

  

 delay(1000-millis()/1000);
}

//void buzz() {
//  ledcWriteTone(channel, freq);
//  ledcWrite(channel, 255);
//  delay(1000);
//}

void servomove(){
  myservo.write(20); 
  Serial.println("Moving Servo");  
  delay(15);
}


void alti()
{
  current_level = (alt - cal_alt);
  if ((alt - cal_alt) < 500.0){
    if (maximum_altitude < current_level){
      maximum_altitude = current_level;
    }
  }
  else{
    release_flag = 1;
  } 
}


void software_state(){
  delta = current_level - previous_level;
  previous_level = current_level;

  if (release_flag == 0){
    if (delta <=1.0 && delta >=-1.0){
    ss = 'G';
    }
   else if (delta >=1.0){
      ss = 'A';
    }
    else if (delta <=-1.0){
      ss = 'D';
    }
  }
  else if (release_flag == 1){
    if ((current_level < 700.0) && (current_level > 40.0)){
      ss = 'P';
    }
    else if (current_level < 40.0){
      ss = 'L';
    }
  }
}

void telemetry()
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  Serial.print("1072");
  Serial.print(",");
  Serial.print(m_time);
  Serial.print(",");
  Serial.print(pkt);
  Serial.print(",");
  Serial.print(mde);
  Serial.print(",");
  Serial.print(ss);
  Serial.print(",");
  Serial.print(current_level);
  Serial.print(",");
  Serial.print(hs_deployed);
  Serial.print(",");
  Serial.print(pc_deployed);
  Serial.print(",");
  Serial.print(mast_raised);
  Serial.print(",");
  Serial.print(bmp.temperature);
  Serial.print(",");
  Serial.print( g.gyro.x- cal_x);
  Serial.print(",");
  Serial.print( g.gyro.y - cal_y);
  Serial.println();
    
}


int release_mechanism()
{
  if (ss == 'P')
  {
    //if(d == 0){     
//---------------- Camera deployment -----------------------------------------------------------------------------------
      //digitalWrite(cam_trigger, LOW);
     // delay(1200);               
     // digitalWrite(cam_trigger, HIGH);
//-----------------------------------------------------------------------------------------------------------------------


      //d = d + 1; 
      //EEPROM.put(16,d);
    //}
  }
           
    if (ss == 'L')
    {    
    //Audio Beacon activation, Camera stop & Telemetry Stop
      //analogWrite(Buzzer_pin,255);
      //digitalWrite(cam_trigger,LOW);
      //EEPROM.write(2,0);
    }
}
