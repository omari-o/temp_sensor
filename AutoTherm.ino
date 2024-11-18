/*
  LiquidCrystal Library - Serial Input

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * LCD VO pin to digital pin 6 - no potentiometer; set value manuallly
*/

// include the library code:
#include <LiquidCrystal.h>
#include "DHT.h"
#include <Wire.h>
#include <DS3231.h>

#define DHTPIN 7     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

RTClib myRTC;

void setup() {
  Serial.begin(57600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  analogWrite(6, 75);
  // initialize the serial communications:
  Wire.begin();
}

void readTemp() {
  lcd.clear();

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  float c = dht.readTemperature();      // Celsius (the default)
  float f = dht.readTemperature(true);  // Fahrenheit (isFahrenheit = true)

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(c) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Computes heat index
  float hif = dht.computeHeatIndex(f, h);         // F (the default)
  float hic = dht.computeHeatIndex(c, h, false);  // C (isFahreheit = false)

  lcd.print("Temp: ");
  lcd.print(f);
  lcd.setCursor(0,1);
  lcd.print("HeIn: ");
  lcd.print(hif);
/*
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(c);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
*/
}

void loop() {
  readTemp();
  delay(1000);

  DateTime now = myRTC.now();
    
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}
