
long Solar(int pin){
  int rawSU;
  float milliVolts;
  float UV;
  
  rawSU = analogRead(pin);

  milliVolts = ((float(rawSU) * 1100)/1023);
  
  UV = milliVolts * 5;
  
  return (long(UV * 100));
}


