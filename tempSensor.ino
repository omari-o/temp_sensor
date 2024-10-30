#include <DHT_U.h>

#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>
#include "DHT.h"

/*
  SD card read/write

This example shows how to read and write data to and from an SD card file

SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK  - pin 13
 ** CS   - pin 4  (for MKRZero SD: SDCARD_SS_PIN)
*/


// defines pins nums/vars
#define DHT11_TYPE DHT11
#define DHT11_PIN 4
#define SD_PIN 5
#define TIME_REQUEST  7     // ASCII bell character requests a time sync message 

DHT dht(DHT11_PIN,DHT11_TYPE);
File myFile;

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  pctime = Serial.parseInt();
  Serial.print("Time: ");
  Serial.println(pctime);

  if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
    setTime(pctime); // Sync Arduino clock to the time received on the serial port
    Serial.print("Time2: ");
    Serial.println(pctime);
  }
}

time_t requestSync() {
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

// prints preceding colon and leading 0 for minutes + seconds
void printDigits(int digits) {
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void setup(){
  Serial.begin(9600);
  dht.begin();

  setSyncProvider(requestSync());  //set function to call when sync required
  Serial.println("Waiting for sync message");
  processSyncMessage();
  Serial.println("Sync complete!");

  /*
  if (!SD.begin(SD_PIN)) {
    Serial.println("initialization failed!\nHit reset to try again");
    while (1);
  } else {
    Serial.println("initialization done.");
  }
  */
}

void loop(){
  // Waits a few seconds btw measurements
  delay(3000); //3 second
  //delay(900000); 15 minutes
  if (timeStatus()!= timeNotSet) {
    time_t t = now(); // Store the current time in time 
    Serial.print("Date: ");
    Serial.print(hour(t));
    Serial.print(":");
    Serial.print(minute(t));
    Serial.print(":");
    Serial.print(second(t));
    Serial.print(".");
    Serial.print(month(t));
    Serial.print(".");
    Serial.print(day(t));
    Serial.print(".");
    Serial.println(year(t));
  }

  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(dayShortStr(weekday()));
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(monthShortStr(month()));
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 

  Serial.print("Temp: ");
  Serial.println(dht.readTemperature(true));

  // Tries opening the file to write data to
/*  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile && timeStatus()!= timeNotSet) {
    printToFile();
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }*/
}