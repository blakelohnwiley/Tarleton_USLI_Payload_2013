/* HIH4030 Humidity Sensor
   Connect to 5V.
References:
  https://www.sparkfun.com/products/9569
  http://www.arduino.cc/forum/index.php/topic,19961.0.html 
  
*/
int HIH4030_readRH(float temp){
  int raw = getValue();
  float RH, TrueRH;
  
  float volts = (float(raw) * 5)/1023;
  
  RH = ((volts/4.8)-0.16)/0.0062;
  
  TrueRH = RH/(1.0546-0.00216*temp);
 
  return int(RH*100);  
}

int getValue(){
  int value;
  
  Serial3.flush();
  
  value = Serial3.parseInt();
  
  Serial3.read();
  
  return value;
}

