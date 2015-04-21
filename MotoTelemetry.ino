
#include "SoftwareSerial.h"


//SoftwareSerial GPSComm(19, 18); // RX, TX
String ver = "0.1";
bool bluetooth_en = false;
bool serial_en = true;
bool gps_en = true;

void setup() 
{
  // Open serial communications and wait for port to open:
  if(serial_en)
  Serial.begin(9600);

  Serial.println("MTS v"+ver);
  // set the data rate for the SoftwareSerial port
  if(gps_en)
  Serial1.begin(9600);

}

void loop() // run over and over
{
  if (Serial1.available())
  //READ GPS Signal
  {
    int data;
    data = Serial1.read();
    parseGPS(data);
   // Serial.write(Serial1.read());
  }
      if (Serial.available())
      {
        Serial1.write(Serial.read());
      }

}

void parseGPS(int gps_data){
  
Serial.write(gps_data);

}
