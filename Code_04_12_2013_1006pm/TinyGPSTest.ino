


void tinyGPS()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    int year;
    byte month, day, hour, minute, second, hundredths;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    latInt = flat * 10000;
    longitudeInt = flon * 10000;
    
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths);
    // Print data and time
    gpsHr = hour;
    gpsMin = minute;
    gpsSec = second;
  
    /*
    Serial2.print("LAT=");
    Serial2.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial2.print(" LON=");
    Serial2.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial2.print(" SAT=");
    Serial2.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial2.print(" PREC=");
    Serial2.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    */
  }
  /*
  gps.stats(&chars, &sentences, &failed);
  Serial2.print(" CHARS=");
  Serial2.print(chars);
  Serial2.print(" SENTENCES=");
  Serial2.print(sentences);
  Serial2.print(" CSUM ERR=");
  Serial2.println(failed);
  */
}
