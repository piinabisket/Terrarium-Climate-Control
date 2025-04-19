#ifndef MAIN_H
#define MAIN_H

#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>    
#include <TouchScreen.h>
#include <EEPROM.h>

#define SERIAL

#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 
#define LCD_RESET A4 

#define TS_MINX 105
#define TS_MINY 85
#define TS_MAXX 970
#define TS_MAXY 940

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//Button Positions
#define TEMP_DOWN_X 220
#define TEMP_DOWN_Y 71
#define TEMP_UP_X 265
#define TEMP_UP_Y 71
#define HUM_DOWN_X 220
#define HUM_DOWN_Y 116
#define HUM_UP_X 265
#define HUM_UP_Y 116

#define BACK_X 265
#define BACK_Y 5
#define NEXT_X 265
#define NEXT_Y 190
#define PREV_X 20
#define PREV_Y 190

#define BUTTON_H 30
#define BUTTON_W 35

//User settings memory locations
#define TS_MEM 0
#define HS_MEM 1

extern MCUFRIEND_kbv tft;
extern TouchScreen ts;

extern TSPoint p;                      //touchscreen datapoint
extern int8_t humidSetpoint;           //setpoints for system to adjust to
extern int8_t tempSetpoint;

void setup();
void loop();

#endif