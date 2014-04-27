// This is a basic snapshot sketch using the VC0706 library.
// On start, the Arduino will find the camera and SD card and
// then snap a photo, saving it to the SD card.
// Public domain.

// If using an Arduino Mega (1280, 2560 or ADK) in conjunction
// with an SD card shield designed for conventional Arduinos
// (Uno, etc.), it's necessary to edit the library file:
//   libraries/SD/utility/Sd2Card.h
// Look for this line:
//   #define MEGA_SOFT_SPI 0
// change to:
//   #define MEGA_SOFT_SPI 1
// This is NOT required if using an SD card breakout interfaced
// directly to the SPI bus of the Mega (pins 50-53), or if using
// a non-Mega, Uno-style board.

#include <Adafruit_VC0706.h>
#include <SD.h>

// comment out this line if using Arduino V23 or earlier
#include <SoftwareSerial.h>         

// uncomment this line if using Arduino V23 or earlier
// #include <NewSoftSerial.h>       

// SD card chip select line varies among boards/shields:
// Adafruit SD shields and modules: pin 10
// Arduino Ethernet shield: pin 4
// Sparkfun SD shield: pin 8
// Arduino Mega w/hardware SPI: pin 53
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
#define chipSelect 10

// Pins for camera connection are configurable.
// With the Arduino Uno, etc., most pins can be used, except for
// those already in use for the SD card (10 through 13 plus
// chipSelect, if other than pin 10).
// With the Arduino Mega, the choices are a bit more involved:
// 1) You can still use SoftwareSerial and connect the camera to
//    a variety of pins...BUT the selection is limited.  The TX
//    pin from the camera (RX on the Arduino, and the first
//    argument to SoftwareSerial()) MUST be one of: 62, 63, 64,
//    65, 66, 67, 68, or 69.  If MEGA_SOFT_SPI is set (and using
//    a conventional Arduino SD shield), pins 50, 51, 52 and 53
//    are also available.  The RX pin from the camera (TX on
//    Arduino, second argument to SoftwareSerial()) can be any
//    pin, again excepting those used by the SD card.
// 2) You can use any of the additional three hardware UARTs on
//    the Mega board (labeled as RX1/TX1, RX2/TX2, RX3,TX3),
//    but must specifically use the two pins defined by that
//    UART; they are not configurable.  In this case, pass the
//    desired Serial object (rather than a SoftwareSerial
//    object) to the VC0706 constructor.

// Using SoftwareSerial (Arduino 1.0+) or NewSoftSerial (Arduino 0023 & prior):

// On Uno: camera TX connected to pin 2, camera RX to pin 3:
SoftwareSerial cameraconnection = SoftwareSerial(2, 3);


Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

// Using hardware serial on Mega: camera TX conn. to RX1,
// camera RX to TX1, no SoftwareSerial object is required:
//Adafruit_VC0706 cam = Adafruit_VC0706(&Serial1);

int j =0;
int k =0;
int z =0;

void setup() {

  // When using hardware SPI, the SS pin MUST be set to an
  // output (even if not connected or used).  If left as a
  // floating input w/SPI on, this can cause lockuppage.

  Serial.begin(9600);
  
  // see if the card is present and can be initialized:
  SD.begin(10);
  
  // Try to locate the camera
  cam.begin();

  // Set the picture size - you can choose one of 640x480, 320x240 or 160x120 
  // Remember that bigger pictures take longer to transmit!
  
  
}

void loop(){
  //cam.setImageSize(VC0706_640x480);        // biggest
  cam.setImageSize(VC0706_320x240);        // medium
  //cam.setImageSize(VC0706_160x120);          // small  
  
  delay(250);

  cam.takePicture();
  
  // Create an image with the name IMAGExx.JPG
  char filename[12];
  strcpy(filename, "00000.JPG");
  for (int i = 0; i < 100; i++) {
    filename[0] = '0' + z;
    filename[1] = '0' + j;
    filename[2] = '0' + k;
    filename[3] = '0' + i/10;
    filename[4] = '0' + i%10;
    if(i==99){
      i=0;
      k++;
      if(k == 9){
        k=0;
        j++;
        if(j == 9){
          j = 0;
          z++;
        }
      }
    }
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  
  // Open the file for writing
  File imgFile = SD.open(filename, FILE_WRITE);
  
  uint16_t jpglen = cam.frameLength();
  
  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen != 0) {
  // read 64 bytes at a time;
  uint8_t *buffer;
  uint8_t bytesToRead = min(32, jpglen);
  buffer = cam.readPicture(bytesToRead);

  imgFile.write(buffer, bytesToRead);
  //Serial.print("Read "); Serial.print(bytesToRead, DEC); Serial.println(" bytes");
  jpglen -= bytesToRead;
  }
  imgFile.close();
  
  cam.resumeVideo();
}

