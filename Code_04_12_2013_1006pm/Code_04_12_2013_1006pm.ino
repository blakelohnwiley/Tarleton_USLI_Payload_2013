/*******************************************************************************************************************/
/*  Tarleton Aeronautical Team
/*  University Student Launch Initiative 2012-2013
/*  Atmospheric Data Gathering Circuit
/*
/*  Created: November 16, 2012
/*
/* CHANGE LOG -
/*
/*  November 16, 2012:
/*     Editor's name: 
/*       JP
/*     Changes:
/*       Set up change log and main structure of the code.
/*       Added the BMP180, TSL2561, and HH10D functionality which all use I2C.
/*       Simply printed readings to the Serial port.
/*
/*  November 17, 2012:
/*     Editor's name:
/*       JP
/*     Changes:
/*       Changed baud rate to 19200 to match the XBee.
/*       Added the microSD and GPS functionality.
/*
/*  November 19, 2012:
/*     Editor's name:
/*       JP
/*     Changes:
/*       Added the HIH4030 functionality.
/*
/*  December 6, 2012:
/*      JP
/*    Changes:
/*      Took out HH10D.
/*      Added timestamp in milliseconds.
/*
/*  December 11, 2012:
/*      Took out a lot of the SD writes.
/*
/*  December 12, 2012:
/*      Added SU100 and SP110
/*      Changed the HIH functioning because the reference
/*      voltage for the Solar sensors is 1.1 and it
/*      is 5V. The ProMini sends readings to the Mega
/*      over Serial2
/*
/*  December 14, 2012:
/*      Added code to read the ADXL from the Mini.
/*
/*  April 09, 2013:
/*      We've been having power issues.
/*      In this edit, I slow down frequency of transmission 
/*      and implement a remote activation.
/*
/********************************************************************************************************************/

// Preprocessing Directives
#include <Wire.h> // I2C Library
#include <Arduino.h> // This is used for Arduino-1.0 for many functions. 
#include "TSL2561.h"
#include <SD.h>   
#include <TinyGPS.h>

#define SERIAL_BAUD_RATE 19200
#define GPS_BAUD_RATE 115200
#define XBEE_BAUD_RATE 19200

// Global Variables
TinyGPS gps;

// HIH4030
int HIH4030_RH;
int HIH4030_PIN = 0;

// TSL2561
int TSL2561_LUX;

// BMP180
short BMP180_TEMP;
long BMP180_PRESSURE;
long BMP180_INIPRESSURE;
long BMP180_ALTITUDE;

// GPS
char GPSBUFFER[100];

// Solar
long SU1;  // pin 0
long SU2;  // pin 1
long SU3;  // pin 2
long SU4;  // pin 3
long SP1;  // pin 4
long SP2;  // Pin 5
long SP3;  // pin 6
long SP4;  // Pin 7

// ADXL
String ADXLBUFFER;

// XBee
String TELEMETRY_STRING = "";

// SD
File DATAFILE;

// Time
long TIMER;

char WIRELESS_COMMAND = '*';

int x = 0;
String testString = " ";

boolean landed = false;
boolean done = false;

unsigned long latInt = 0;
long longitudeInt = 0;
long gpsaltitude = 0;
byte gpsHr, gpsMin, gpsSec;


// Setup
void setup(){
  // XBee Serial connections.
  Serial3.begin(19200); 
  Serial.begin(19200);
  Serial2.begin(19200);

  // GPS Serial Connections.
  Serial1.begin(19200);

  // Arduino Mini HIH4030 Connection
  //Serial2.begin(19200);

  Wire.begin(); 


  // Initiates the SD card using cs pin 53
  if(SD.begin(53)){
    // Create the SD file and write a title                         //   
    DATAFILE = SD.open("datalog.txt", FILE_WRITE);             //
    DATAFILE.println();
    DATAFILE.println(); 
    DATAFILE.println("System Initializing..."); 
  }    

  //Calibration variables needed for the BMP180 to give accurate data. 
  TSL2561_calibration();

  bmp180Calibration();       //Caliberation variables needed for the BMP180 to give accurate data.
  Serial.println("bmp calibration");
  BMP180_TEMP = bmp180GetTemperature(bmp180ReadUT());
  BMP180_INIPRESSURE = bmp180GetPressureIni(bmp180GetPressure(bmp180ReadUP()));  


  // Setting the refernece voltage because the Solar sensors output
  // such a low voltage, and the ADC is only 10 bit.
  analogReference(INTERNAL1V1);
  
  //Serial2.println("End of setup");
}

// Loop
void loop(){
  WIRELESS_COMMAND = Serial2.read();
  
  if(WIRELESS_COMMAND == 'l'){
    if(landed == true){
      landed = false;
    }
    else {
      landed = true;
    }
  }
  
  if(WIRELESS_COMMAND == 'd'){
    if(done == true){
      done = false;
    }
    else {
      done = true;
    }
  }
  
  if(landed == true){
    delay(8000);
  }
  
  if(done == true){
    while(done){
      WIRELESS_COMMAND = Serial2.read();
      if(WIRELESS_COMMAND == 'd'){
        done = false;
      }
      delay(150);
    }
  }

  //Serial2.println("1");
  // Read lux value from the TSL2561.

  TSL2561_LUX = TSL2561_getLux();
  // Serial2.println("2");

  BMP180_TEMP     = bmp180GetTemperature(bmp180ReadUT());  // temperature must be populated prior to pressure

  BMP180_PRESSURE = bmp180GetPressure(bmp180ReadUP());       // pressure must be calculated before altitude

  BMP180_ALTITUDE = bmp180GetAltitude(BMP180_PRESSURE,BMP180_INIPRESSURE);

  // Function parameters are pin # and temperature.
  HIH4030_RH = HIH4030_readRH(float(BMP180_TEMP)/10);
  // Serial2.println("Pre GPS");
  // Gathers GPS data and stores them to GPSBUFFER
  //GPS();
  tinyGPS();
  //Serial2.println("Post GPS");

  // Solar readings
  SU1 = Solar(0);
  SU2 = Solar(1);
  SU3 = Solar(2);
  SU4 = Solar(3);
  SP1 = Solar(4);
  SP2 = Solar(5);
  SP3 = Solar(6);
  SP4 = Solar(7);

  // Accelerometer
  // ADXL();

  // Save the current time since program started.
  TIMER = millis();
  // Construct the string that is transmited via the XBee.

  TELEMETRY_STRING = " ";
  TELEMETRY_STRING += HIH4030_RH;

  TELEMETRY_STRING += " ";
  TELEMETRY_STRING += TSL2561_LUX;

  TELEMETRY_STRING += " ";
  TELEMETRY_STRING += BMP180_TEMP;

  TELEMETRY_STRING += " ";
  TELEMETRY_STRING += BMP180_PRESSURE;

  TELEMETRY_STRING += " ";
  TELEMETRY_STRING += BMP180_ALTITUDE;

  TELEMETRY_STRING += " ";
  TELEMETRY_STRING += gpsHr;
  TELEMETRY_STRING += gpsMin;
  TELEMETRY_STRING += gpsSec;
  
  TELEMETRY_STRING += " ";
  TELEMETRY_STRING += latInt;
  
  TELEMETRY_STRING += " ";
  TELEMETRY_STRING += longitudeInt;
  
  TELEMETRY_STRING += " ";  
  TELEMETRY_STRING += SU1;
  TELEMETRY_STRING += " ";  
  TELEMETRY_STRING += SU2;
  TELEMETRY_STRING += " ";  
  TELEMETRY_STRING += SU3;
  TELEMETRY_STRING += " ";  
  TELEMETRY_STRING += SU4;

  TELEMETRY_STRING += " ";  
  TELEMETRY_STRING += SP1;
  TELEMETRY_STRING += " ";  
  TELEMETRY_STRING += SP2;
  TELEMETRY_STRING += " ";  
  TELEMETRY_STRING += SP3;
  TELEMETRY_STRING += " ";  
  TELEMETRY_STRING += SP4;

  //TELEMETRY_STRING += " "; 
  //TELEMETRY_STRING += ADXLBUFFER;

  TELEMETRY_STRING += " "; 

  Serial2.flush();

  // Transmit
  delay(100);

  Serial2.println(TELEMETRY_STRING); 

  delay(100);
  TELEMETRY_STRING += TIMER;
  TELEMETRY_STRING += " ";  
  //Serial2.println("5");
  DATAFILE = SD.open("datalog.txt", FILE_WRITE);  
  DATAFILE.println(TELEMETRY_STRING); 
  DATAFILE.close();  


  delay(100);
  //Serial2.println("End of loop");
}






