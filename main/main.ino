#include "main.h"
#include "display.h"

#include <Wire.h>
#include <I2C_RTC.h>

MCUFRIEND_kbv tft;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int period = 20;                //limit screen refresh to conserve power
int timeout = 100000;              //timeout value
unsigned long time_now = 0;

static DS3231 RTC;

TSPoint p;                      //touchscreen datapoint
int8_t humidSetpoint;           //setpoints for system to adjust to
int8_t tempSetpoint;
int8_t humidIntervalSetpoint;

int8_t second;
int8_t minute;
int8_t hour;
int8_t day;
int8_t month;

int8_t alarmPeriod = 5;         //alarm period 1 in seconds
bool alarm = false;             //alarm 1 flag
int8_t nextAlarm;

bool tempButtonState = true;    //flag to indicate if button has been pressed
bool humidButtonState = true;
bool configButtonState = true;
bool homeButtonState = true;
bool intButtonState = true;
int screen = 0;                 //flag to indicate what screen is being rendered

void setup() {
  Serial.begin(9600);
  tft.reset();
  tft.begin(tft.readID());
  tft.setRotation(3);
  delay(500);
  Serial.println(tft.readID(), HEX);
  tft.fillScreen(BLACK);
  setupRTC();
  #ifdef INIT_MEM
    EEPROM.write(TS_MEM, 72);
    EEPROM.write(HS_MEM, 72);
    EEPROM.write(HIS_MEM, 30);
  #endif
  //Read setpoints from EEPROM
  tempSetpoint = EEPROM.read(TS_MEM);
  humidSetpoint = EEPROM.read(HS_MEM);
  humidIntervalSetpoint = EEPROM.read(HIS_MEM);
}

void loop() 
{
  time_now = millis();  //get current time
  // Delay between measurements.
  while(millis() < time_now + period){
    TSPoint p = ts.getPoint();  //Get touch point
    if(timeout){timeout--;}
    if (p.z > ts.pressureThreshhold) {
      timeout = 10000;
      #ifdef SERIAL
      Serial.print("X = "); Serial.print(p.x);
      Serial.print("\tY = "); Serial.print(p.y);
      Serial.print("\n");
      #endif
      p.x = map(p.x, TS_MINX, TS_MAXX, 0, 320);
      p.y = map(p.y, TS_MINY, TS_MAXY, 0, 240);
      //Detect button presses on config screen
      if(screen){   
        //Next config screen button
        if(checkButton(p, NEXT_X, NEXT_Y, configButtonState, rightButton, *nextButtonCallback)){continue;}
        //Prev config screen button
        else if(checkButton(p, PREV_X, PREV_Y, configButtonState, leftButton, *prevButtonCallback)){continue;}
        //Return from config to home button
        else if(checkButton(p, BACK_X, BACK_Y, configButtonState, backButton, *backButtonCallback)){continue;}

        switch(screen){
          case SCREEN_CONF_SETPOINTS:
            //Temp Setpoint DOWN button pressed
            if(checkButton(p, TEMP_DOWN_X, TEMP_DOWN_Y, tempButtonState, minusButton, *tempDownButtonCallback)){continue;}
            //Temp Setpoint UP button pressed
            else if(checkButton(p, TEMP_UP_X, TEMP_UP_Y, tempButtonState, plusButton, *tempUpButtonCallback)){continue;}
            //Humidity UP button pressed
            else if(checkButton(p, HUM_UP_X, HUM_UP_Y, humidButtonState, plusButton, *humidUpButtonCallback)){continue;}
            //Humidity DOWN button pressed
            else if(checkButton(p, HUM_DOWN_X, HUM_DOWN_Y, humidButtonState, minusButton, *humidDownButtonCallback)){continue;}
            break;

          case SCREEN_CONF_TIMES:
            break;

          case SCREEN_CONF_INTERVALS:
            //Humidity Interval UP button pressed
            if(checkButton(p, INT_UP_X, INT_UP_Y, intButtonState, plusButton, *humidIntervalUpButtonCallback)){continue;}
            //Humidity Interval DOWN button pressed
            else if(checkButton(p, INT_DOWN_X, INT_DOWN_Y, intButtonState, minusButton, *humidIntervalDownButtonCallback)){continue;}
            break;
          
          default:
            break;
        }
      }
      //Home Screen buttons
      else{
        //Home screen to config screen button
        if(checkButton(p, BACK_X, BACK_Y, homeButtonState, configMenuButton, *homeButtonCallback)){continue;}
      }
    }
  }
  if(!timeout){
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    tft.fillScreen(BLACK);
    drawButtons();
  }
  else if(screen){
    displayConfig();
  }
  else{
    checkSensors();
    displayHome();
  }
}

void setupRTC(){
  RTC.begin();
  second = RTC.getSeconds();
  nextAlarm = adjustAlarm(alarmPeriod + second);
  // RTC.setHourMode(CLOCK_H12);
  // RTC.setMeridiem(HOUR_AM);
  // RTC.setYear(2025);
  // RTC.setMonth(5);
  // RTC.setDay(23);
  // RTC.setHours(11);
  // RTC.setMinutes(45);
  // RTC.setSeconds(0);

}

void checkSensors(){
  month = RTC.getMonth();
  day = RTC.getDay();
  hour = RTC.getHours();
  minute = RTC.getMinutes();
  second = RTC.getSeconds();
  meridiem = RTC.getMeridiem()
  if(second == nextAlarm){
    nextAlarm = adjustAlarm(alarmPeriod + second);
    alarm = true;
  }
  else{
    alarm = false;
  }
}

int8_t adjustAlarm(int8_t time){
  if(time > 60)  return time - 60;
  return time;
}

void lightHandler(){
  start = convertTime24(lightOn, lightOnMer);
  end = convertTime24(lightOff, lightOffMer);
  currHour = convertTime24(hour, meridiem);
  if()
    if((currHour >= start) && (currHour < end)){
      digitalWrite(LIGHT_PIN, HIGH);
    }
    else{
      digitalWrite(LIGHT_PIN, LOW);
    }
}
int convertTime24(int8_t hour, int meridiem){
  if(meridiem == HOUR_AM){
    return hour;
  }
  return hour + 12;
}
/*
 *  @brief  Writes user settings to EEPROM if settings changed
 */
void updateSettings(){
  if(humidIntervalSetpoint != EEPROM.read(HIS_MEM)){
    EEPROM.write(HIS_MEM, humidIntervalSetpoint);
  }
  if(tempSetpoint != EEPROM.read(TS_MEM)){
    EEPROM.write(TS_MEM, tempSetpoint);
  }
  if(humidSetpoint != EEPROM.read(HS_MEM)){
    EEPROM.write(HS_MEM, humidSetpoint);
  }
}

