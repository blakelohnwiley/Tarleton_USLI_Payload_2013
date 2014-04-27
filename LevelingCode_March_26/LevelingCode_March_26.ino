#include <Wire.h>
#include <Servo.h>

#include "BASIC_ADXL345.h"

Servo xServo;
Servo yServo;

int xAxis,yAxis,zAxis;

char input;

int HIH;

String tString;

void setup(){
  Serial.begin(19200);
  
  analogReference(DEFAULT);
  
  Wire.begin();
  delay(100);
  
  //Sets the range for the ADXL345 between 2G,4G,8G,16G.
  SetRange(16, true);
  
  // Tells the ADXL345 to begin taking measurements. This is essensial.
  EnableMeasurementsADXL();  
 
  xServo.attach(7);
  yServo.attach(6);

  xServo.write(90);
  yServo.write(90);
 
}

void loop(){
  
  input = Serial.read();
  delay(150);
 
  //reads the x,y,z axis from the ADXL345 and stores them into appropriate varables
  xAxis = ReadXAxis();
  yAxis = ReadYAxis();
  zAxis = ReadZAxis();
  
  // Sending accelerometer data.
 
  
  // Sending humidity data.
  //if(input == 'H'){
    HIH = analogRead(0);
    
    tString = " ";
    tString += HIH;
    tString += "e";
    
    Serial.print(tString);
  //}
  
  
  // If you change the threshold
  // the servo will not move until
  // the accelerometer reads that value.
  
  // If you change the number the xAxis is divided
  // by then the speed the servo motors move
  // will change.
  
  if(abs(xAxis) > 25){
    if((xServo.read() - (xAxis/15)) < 10 || (xServo.read() - (xAxis/15)) > 160)
    {
      
    } 
    else
    {
      xServo.write(xServo.read() - (xAxis/15));
    } 
  }
  
  if(abs(yAxis) > 25){
    if((yServo.read() - (yAxis/15)) < 10 || (yServo.read() - (yAxis/15)) > 160)
    {
      
    } 
    else
    {
      yServo.write(yServo.read() - (yAxis/15));
    } 
  }
    
}

