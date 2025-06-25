#ifndef EXTERNLIB_H
#define EXTERNLIB_H

#include <I2C_RTC.h>
#include <Wire.h>
#include <Adafruit_GFX.h>    
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include <EEPROM.h>

extern MCUFRIEND_kbv tft;
extern TouchScreen ts;
extern TSPoint p;                      //touchscreen datapoint
extern DS3231 RTC;

extern int8_t humidSetpoint;           //setpoints for system to adjust to
extern int8_t tempSetpoint;
extern int8_t humidIntervalSetpoint;

extern bool tempButtonState;
extern bool humidButtonState;
extern bool intButtonState;
extern bool configButtonState;
extern bool homeButtonState;
extern int screen;

extern int8_t second;
extern int8_t hour;
extern int8_t minute;
extern int8_t day;
extern int8_t month;

extern int8_t timer1;
extern int8_t timer2;
extern int8_t timeoutTimer;

#endif