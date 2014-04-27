#define ADXL345_Address 0x1D
#define Register_PowerControl 0x2D
#define Register_DataFormat 0x31
#define Register_DataX 0x32
#define Register_DataY 0x34
#define Register_DataZ 0x36

#define ScaleFor2G 0.0039
#define ScaleFor4G 0.0078
#define ScaleFor8G 0.0156
#define ScaleFor16G 0.0312

#define ErrorCode_1 "Entered range was invalid. Should be 2, 4, 8 or 16g."
#define ErrorCode_1_Num 1



//Writes data to address via I2C
//////////////////////////////////////////////
uint8_t long Write(int address,int data)    //
{                                           // 
  Wire.beginTransmission(ADXL345_Address);  //
  Wire.write(address);                      //  
  Wire.write(data);                         // 
  Wire.endTransmission();                   // 
}                                           // 
//////////////////////////////////////////////

///Reads data via I2C
//////////////////////////////////////////////////////
uint8_t* Read(int address, int length)              //
{                                                   // 
  Wire.beginTransmission(ADXL345_Address);          //
	Wire.write(address);                        //
	Wire.endTransmission();                     // 
                                                    //
	Wire.beginTransmission(ADXL345_Address);    //
	Wire.requestFrom(ADXL345_Address, length);  //  
                                                    //
	uint8_t buffer[length];                     // 
	if(Wire.available() == length)              //
	{                                           // 
		for(uint8_t i = 0; i < length; i++) // 
		{                                   // 
			buffer[i] = Wire.read();    //
		}                                   //   
	}                                           // 
	Wire.endTransmission();                     // 
                                                    //
	return buffer;                              //
}                                                   // 
//////////////////////////////////////////////////////

// Sets the Range. 2G,4G,8G,16G options available.
///////////////////////////////////////////////////////////////////////////
int SetRange(int range, bool fullResolution)                             // 
{                                                                        //
  float m_Scale;                                                         // 
                                                                         // 
	// Get current data from this register.                          //  
	uint8_t data = Read(Register_DataFormat, 1)[0];                  //
                                                                         // 
	// We AND with 0xF4 to clear the bits are going to set.          //
	// Clearing ----X-XX                                             // 
	data &= 0xF4;                                                    //
                                                                         //   
	// By default (range 2) or FullResolution = true, scale is 2G.   // 
	m_Scale = ScaleFor2G;                                            //  
	                                                                 // 
	// Set the range bits.                                           // 
	switch(range)                                                    //    
	{                                                                //    
		case 2:                                                  //
			break;                                           // 
		case 4:                                                  //
			data |= 0x01;                                    //
			if(!fullResolution) { m_Scale = ScaleFor4G; }    //
			break;                                           //   
		case 8:                                                  //
			data |= 0x02;                                    //  
			if(!fullResolution) { m_Scale = ScaleFor8G; }    //
			break;                                           // 
		case 16:                                                 // 
			data |= 0x03;                                    //
			if(!fullResolution) { m_Scale = ScaleFor16G; }   // 
			break;                                           // 
		default:                                                 // 
			return ErrorCode_1_Num;                          //
	}                                                                //
                                                                         // 
	// Set the full resolution bit.                                  //
	if(fullResolution)                                               // 
		data |= 0x08;                                            //
                                                                         // 
	Write(Register_DataFormat, data);                                //  
}                                                                        //
///////////////////////////////////////////////////////////////////////////

// Reads the RAW X axis.
///////////////////////////////////////////////
int ReadXAxis()                           // 
{                                             // 
  uint8_t* buffer = Read(Register_DataX, 2);  //
  int xAxis;                              //
  xAxis = (buffer[1] << 8) | buffer[0];       // 
  return xAxis;                               // 
}                                             // 
                                              //
// Reads the RAW Y axis.                      //
////////////////////////////////////////////////
int ReadYAxis()                           // 
{                                             // 
  uint8_t* buffer = Read(Register_DataY, 2);  //  
  int yAxis;                              //  
  yAxis = (buffer[1] << 8) | buffer[0];       // 
  return yAxis;                               // 
}                                             //   
                                              //
// Reads the Raw Z axis.                      //
////////////////////////////////////////////////
int ReadZAxis()                           //   
{                                             // 
  uint8_t* buffer = Read(Register_DataZ, 2);  //
  int zAxis;                              //
  zAxis = (buffer[1] << 8) | buffer[0];       // 
  return zAxis;                               // 
}                                             // 
////////////////////////////////////////////////

//Tells ADXL to start taking measurements!!!
////////////////////////////////////////////
int EnableMeasurementsADXL()              //                
{                                         // 
  Write(Register_PowerControl, 0x08);     // 
}                                         //
////////////////////////////////////////////
