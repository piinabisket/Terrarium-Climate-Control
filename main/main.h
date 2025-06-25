#ifndef MAIN_H
#define MAIN_H

#include "externlib.h"

/* Uncomment to enable serial outputs for debug */
#define SERIAL
/* Uncomment to set memory to initial values
 * Must be triggered once on first bootup
 * Must be uncommented to properly save user settings
 */
//#define INIT_MEM

/* Touchscreen minimums and maximums, adjust to display */
#define TS_MINX 95
#define TS_MINY 90
#define TS_MAXX 970
#define TS_MAXY 940

#define YP A2  // must be an analog pin
#define XM A3  // must be an analog pin
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

/* Button Positions */
#define TEMP_DOWN_X 220
#define TEMP_DOWN_Y 71
#define TEMP_UP_X 265
#define TEMP_UP_Y 71
#define HUM_DOWN_X 220
#define HUM_DOWN_Y 116
#define HUM_UP_X 265
#define HUM_UP_Y 116
#define INT_UP_X 265
#define INT_UP_Y 80
#define INT_DOWN_X 175
#define INT_DOWN_Y 80
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
#define HIS_MEM 2

//Sensor pins
#define LIGHT_PIN 33
#define HEAT_PIN 34

void setup();
void loop();
void updateSettings();

#endif