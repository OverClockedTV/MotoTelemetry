
#include "SoftwareSerial.h"
#include <SPI.h>
#include <SD.h>

//Global set up variables
String ver = "0.1alpha";
bool bt_en = false;
int bt_baud = 9600;
bool serial_en = true;
int serial_baud = 9600;
//GPS Settings
bool gps_en = true;
int gps_baud = 9600;
bool sd_en = true;
String gps_string = "";
bool gps_complete = false;
String gps_filter_type = "$GPGGA";
int gps_filter_num = 1;
int red_led = 10;
int blue_led = 8;
int green_led = 9;
int bt_pin = 28;
const int SD_PIN =27;


bool gps_reading = false;

void setup()
{
  pinMode(bt_pin, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  digitalWrite(bt_pin, LOW);


 // Open serial communications and wait for port to open:
  if (serial_en)
    enableSerial();

  Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(SD_PIN)) {
    Serial.println("Card failed, or not present");
    digitalWrite(red_led, HIGH);
    return;
    // don't do anything more:
    
  }
  else
  Serial.println("Card initialized.");

 

  // set the data rate for the SoftwareSerial port
  if (gps_en)
    enableGPS();
    
    if(bt_en)
    enableBT();

}

void loop() // run over and over
{
  //READ GPS
  if ( Serial1.available())
  {
    parseGPS(Serial1.read());
  }
  //WRITE TO GPS FROM SERIAL
  if (Serial.available())
  {
    Serial1.write(Serial.read());
  }
  if(Serial2.available())
  {
//    Serial.write(Serial2.read());
  }

}
void enableBT()
{
  digitalWrite(bt_pin, HIGH);
  Serial2.begin(bt_baud);
}
void enableGPS()
{
  Serial1.begin(gps_baud);
}
void enableSerial()
{
  Serial.begin(serial_baud);
  Serial.println("MTS v" + ver);
}
void readGPS(int filter)
{
  // 1 - GPGGA
  
  switch (filter)
  {
    case 1:
      parseGGA();

      break;

    default:
      break;
  }
}
void parseGGA()
{
      String output = "";

  if(gps_string.length() > 50)
  {
          digitalWrite(green_led, HIGH);

    String lat = gps_string.substring(17,27);
    String lat_d = gps_string.substring(28,29);
    String lon = gps_string.substring(31,41);
    String lon_d = gps_string.substring(42,43);
    String time = gps_string.substring(8,13);
    String alt = gps_string.substring(54,58);
    output = time+"\t"+lat+lat_d+ "\t"+lon+lon_d+"\t"+alt+"M";
    
    
      }
  else
{
output = "NO SIGNAL";
}
  digitalWrite(red_led, HIGH);
     File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(output);
    dataFile.close();
    digitalWrite(red_led, LOW);
    // print to the serial port too:
    Serial.println(output);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

        digitalWrite(green_led, LOW);
        
        if(bt_en)
      {
        digitalWrite(blue_led, HIGH);
        Serial2.println(output);
        digitalWrite(blue_led, LOW);
      }


}
void parseGPS(int gps_data) {

  if (!gps_reading)
  {
    if ((char)gps_data == '$')
    {
      gps_string = "";
      gps_reading = true;
      

    }
  }

  if (gps_reading)
  {
    if ((char)gps_data == '\n')
    {
      gps_reading = false;
      if(gps_string.substring(0,6) == gps_filter_type)
      readGPS(gps_filter_num);
    }
    else
    {
         
          gps_string += (char)gps_data;

    }
  }

}
