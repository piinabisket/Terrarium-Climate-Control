#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include "relay.h"

#define DHTPIN1 7     // Digital pins connected to the DHT sensor 
#define DHTPIN2 6
#define RLAYPIN1 5   // Digital pins connected to relays 1-4
#define RLAYPIN2 4
#define RLAYPIN3 3
#define RLAYPIN4 2

#define DHTTYPE    DHT22     // DHT 22 (AM2302)

#define _SERIAL    //define to enable serial communication

DHT_Unified dht1(DHTPIN1, DHTTYPE);   //initialize temp/humidity sensors
DHT_Unified dht2(DHTPIN2, DHTTYPE);
Relay r1(RLAYPIN1);   //initialize relay modules
Relay r2(RLAYPIN2);
Relay r3(RLAYPIN3);
Relay r4(RLAYPIN4);

//uint32_t delayMS;
int period = 10000;
unsigned long time_now = 0;

void setup() {
  Serial.begin(9600);
  // Initialize devices.
  dht1.begin();
  dht2.begin();
  r1.begin();
  r2.begin();
  r3.begin();
  r4.begin();
  delay(2000);
}

void loop() {
  time_now = millis();  //get current time
  // Delay between measurements.
  while(millis() < time_now + period){
      // add the code you want to keep running here
      //wait approx. [period] ms
  }
  // Get temperature event
  sensors_event_t event1;
  sensors_event_t event2;
  dht1.temperature().getEvent(&event1);
  dht2.temperature().getEvent(&event2);
  // Get humidity event
  dht1.humidity().getEvent(&event1);
  dht2.humidity().getEvent(&event2);
  #ifdef _SERIAL
  Serial.println(F("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"));
  Serial.println(F("#======================================================#"));
  if (isnan(event1.temperature) || isnan(event2.temperature)) {
    Serial.println(F("# Error reading temperature!"));
  }
  else {
    Serial.print(F("# Temperature: "));
    Serial.print(convertCtoF(avg(event1.temperature, event2.temperature)));
    Serial.println(F("°F"));
  }
  if (isnan(event1.relative_humidity) || isnan(event2.relative_humidity)) {
    Serial.println(F("# Error reading humidity!"));
  }
  else {
    Serial.print(F("# Humidity: "));
    Serial.print(avg(event1.relative_humidity, event2.relative_humidity));
    Serial.println(F("%"));
  }
  Serial.print(F("# Relay 1: "));
  Serial.print(r1.status() ? F("ON"):F("OFF"));
  Serial.print(F(" Relay 2: "));
  Serial.print(r2.status() ? F("ON"):F("OFF"));
  Serial.print(F(" Relay 3: "));
  Serial.print(r3.status() ? F("ON"):F("OFF"));
  Serial.print(F(" Relay 4: "));
  Serial.println(r4.status() ? F("ON"):F("OFF"));
  Serial.println(F("#======================================================#"));
  #endif
}
/*
Converts celcius to fahrenheit.

Params: float celcius

Returns: float Fahrenheit
*/
float convertCtoF(float c) { return c * 1.8 + 32; }

/*
Calculates the average of two floats.

Params: float, float

Returns: Average float
*/
float avg(float f1, float f2) { return (float)((f1 + f2)/2);}

