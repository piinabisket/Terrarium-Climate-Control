#include "display.h"

/*
 * @brief Display home screen
 */
void displayHome(){
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  //Animate home page buttons
  if(homeButtonState){
    tft.fillScreen(BLACK);
    drawButtons();
    tft.fillRect(NEXT_X, NEXT_Y, BUTTON_W, BUTTON_H, BLACK);
    tft.drawBitmap(BACK_X, BACK_Y, configMenuButton, BUTTON_W, BUTTON_H, WHITE);
  }
  homeButtonState = false;
  tft.setCursor(6,8);
  tft.setTextSize(3);
  tft.setTextColor(WHITE, BLACK);
  tft.print(hour);
  tft.print(":");
  tft.print(minute);
  tft.print(":");
  tft.print(second);
  tft.print("  ");
  tft.print(day);
  tft.print("/");
  tft.print(month);
  tft.println("\n ");
  tft.print(alarm);
  tft.drawRect(0,0,320,240,WHITE);
}
/*
 * @brief Display config screen
 */
void displayConfig(){
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //Animate config page buttons 
  if(configButtonState){
    tft.fillScreen(BLACK);
    drawButtons();
    tft.fillRect(NEXT_X, NEXT_Y, BUTTON_W, BUTTON_H, BLACK);
    tft.drawBitmap(NEXT_X, NEXT_Y, rightButton, BUTTON_W, BUTTON_H, WHITE);    //Next page button
    tft.fillRect(PREV_X, PREV_Y, BUTTON_W, BUTTON_H, BLACK);
    tft.drawBitmap(PREV_X, PREV_Y, leftButton, BUTTON_W, BUTTON_H, WHITE);    //Previous page button
    tft.fillRect(BACK_X, BACK_Y, BUTTON_W, BUTTON_H, BLACK);
    tft.drawBitmap(BACK_X, BACK_Y, backButton, BUTTON_W, BUTTON_H, WHITE);    //Back Button
  }
  configButtonState = false;
  //Draw Header
  tft.setCursor(6, 8);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  tft.print("Config");
  tft.setCursor(142,200);
  tft.setTextSize(2);
  tft.print(screen);
  tft.print("/3");
  tft.drawFastHLine(0, 40, 320, WHITE);
  tft.drawRect(0,0,320,240,WHITE);

  //Temperature and humidity setpoint screen
  if(screen == SCREEN_CONF_SETPOINTS){
    drawConfig1();
  }
  //Light and time config screen
  else if(screen == SCREEN_CONF_TIMES){
    drawConfig2();
  }
  //Interval adjustment config screen
  else if(screen == SCREEN_CONF_INTERVALS){
    drawConfig3();
  }
}

/*
 *  @brief: Draw setpoints config screen
 */
void drawConfig1(){
  tft.setTextSize(2);
  tft.setCursor(6, 49);
  tft.println("Setpoints:");
  tft.setCursor(24, 79);
  tft.print("Temp.     ");
  tft.print(tempSetpoint);
  tft.write(0xF7);
  tft.println("F ");
  tft.setCursor(24, 125);
  tft.print("Humidity  ");
  tft.print(humidSetpoint);
  tft.println("% ");
  //Animate temperature adjustment buttons
  if(humidButtonState){
    tft.fillRect(TEMP_DOWN_X, TEMP_DOWN_Y, BUTTON_W, BUTTON_H, BLACK);
    tft.drawBitmap(TEMP_DOWN_X, TEMP_DOWN_Y, minusButton, BUTTON_W, BUTTON_H, WHITE); //Temperature down
    tft.fillRect(TEMP_UP_X, TEMP_UP_Y, BUTTON_W, BUTTON_H, BLACK);
    tft.drawBitmap(TEMP_UP_X, TEMP_UP_Y, plusButton, BUTTON_W, BUTTON_H, WHITE);      //Temperature up
  }
  //Animate humidity adjustment buttons
  if(humidButtonState){
    tft.fillRect(HUM_DOWN_X, HUM_DOWN_Y, BUTTON_W, BUTTON_H, BLACK);
    tft.drawBitmap(HUM_DOWN_X, HUM_DOWN_Y, minusButton, BUTTON_W, BUTTON_H, WHITE); //Humidity down
    tft.fillRect(HUM_UP_X, HUM_UP_Y, BUTTON_W, BUTTON_H, BLACK);
    tft.drawBitmap(HUM_UP_X, HUM_UP_Y, plusButton, BUTTON_W, BUTTON_H, WHITE);      //Humidity up
  }
  humidButtonState = false;
  tempButtonState = false;
}

/*
 *  @brief: Draw time and light config screen
 */
void drawConfig2(){
  tft.setTextSize(2);
  tft.setCursor(6, 49);
  tft.print("Light and Time:");
}

/*
 *  @brief: Draw intervals config screen
 */
void drawConfig3(){
  tft.setTextSize(2);
  tft.setCursor(6, 49);
  tft.print("Intervals:");
  tft.setCursor(24, 79);
  tft.print("Humidifier ");
  tft.setCursor(24, 99);
  tft.print("on for:   ");
  tft.setCursor(220, 89);
  tft.print(humidIntervalSetpoint);
  tft.println("s");
  tft.setTextSize(1);
  tft.setCursor(32, 125);
  tft.println("  Humidifier engages every 5 minutes until");
  tft.setCursor(32, 135);
  tft.println("         humidity reaches setpoint");
  //Animate adjustment buttons
  if(intButtonState){
    tft.fillRect(INT_DOWN_X, INT_DOWN_Y, BUTTON_W, BUTTON_H, BLACK);
    tft.drawBitmap(INT_DOWN_X, INT_DOWN_Y, minusButton, BUTTON_W, BUTTON_H, WHITE); //Interval down
    tft.fillRect(INT_UP_X, INT_UP_Y, BUTTON_W, BUTTON_H, BLACK);
    tft.drawBitmap(INT_UP_X, INT_UP_Y, plusButton, BUTTON_W, BUTTON_H, WHITE);      //Interval up
  }
  intButtonState = false;
}

/*
 * @brief Simple function to refresh buttons on display by changing each buttons
 *        flag
 */
void drawButtons(){
  humidButtonState = true;
  tempButtonState = true;
  configButtonState = true;
  homeButtonState = true;
  intButtonState = true;
}
/*
 *  @brief  Checks to see if a given button is pressed, and 
 *          executes a given function if pressed.
 *  @params p: Touchscreen object
 *          x, y: Button top left corner coordinates
 *          &state: Dereferenced button state variable, tells other
 *                       used for debouncing and reference in other aspects of the program
 *          *bitmap: Pointer to a bitmap in PROGMEM
 *          *func: Callback function thats called on button press
 *  @returns true: Button has been pressed
 *           false: Button has not been pressed   
 */
bool checkButton(TSPoint p, int x, int y, bool &state, 
            unsigned char *bitmap, void (*func)()){

  if(p.x>x && p.x<x + BUTTON_W && p.y>y && p.y<y + BUTTON_H){
    if(!state){
      func();
    }
    state = true;
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    tft.fillRect(x, y, BUTTON_W, BUTTON_H, WHITE);
    tft.fillRect(x + 1, y + 8, BUTTON_W - 2, BUTTON_H - 9, BLACK);
    drawInvertBitmap(x, y + 3, bitmap, BUTTON_W, BUTTON_H - 3, WHITE);  
    return true;
  }
  return false;
}
/*
 *  @brief  Callback function: raises temp setpoint
 */
void tempUpButtonCallback(){
  tempSetpoint++;
}

/*
 *  @brief  Callback function: lowers temp setpoint
 */
void tempDownButtonCallback(){
  tempSetpoint--;
}

/*
 *  @brief  Callback function: raises humidity setpoint, does
 *          not exceed 100(% humidity)
 */
void humidUpButtonCallback(){
  if(humidSetpoint < 100){
    humidSetpoint++;
  }
}

/*
 *  @brief  Callback function: lowers humidity setpoint, does
 *          not exceed 0(% humidity)
 */
void humidDownButtonCallback(){
  if(humidSetpoint > 0){
    humidSetpoint--;
  }
}

/*
 *  @brief  Callback function: lowers humidity interval, does
 *          not exceed 0 seconds
 */
void humidIntervalDownButtonCallback(){
  if(humidIntervalSetpoint > 15){
    humidIntervalSetpoint -= 15;
  }
}

/*
 *  @brief  Callback function: lowers humidity setpoint, does
 *          not exceed 60 seconds
 */
void humidIntervalUpButtonCallback(){
  if(humidIntervalSetpoint < 60){
    humidIntervalSetpoint += 15;
  }
}

/*
 *  @brief  Callback function: sets screen to config 1
 */
void homeButtonCallback(){
  screen = SCREEN_CONF_SETPOINTS;
}

/*
 *  @brief  Callback function: sets screen to home; writes
 *          setpoints to memory if changed
 */
void backButtonCallback(){
  screen = SCREEN_HOME;
  updateSettings();
}

/*
 *  @brief  Callback function: sets screen to next config screen
 */
void nextButtonCallback(){
  if(screen == SCREEN_CONF_INTERVALS){
    screen = SCREEN_CONF_SETPOINTS;
  }
  else{
    screen++;
  }
}

/*
 *  @brief  Callback function: sets screen to previous config screen
 */
void prevButtonCallback(){
  if(screen == SCREEN_CONF_SETPOINTS){
    screen = SCREEN_CONF_INTERVALS;
  }
  else{
    screen--;
  }
}

/*
 *  @brief  Draws bitmap to TFT with inverted colors: 0 = pixel
 *  @params x, y: Coordinates of upper left corner of bitmap position
 *          *bitmap: Pointer to bitmap in PROGMEM
 *          w, h: Bitmap width and height
 *          color: Two-byte color to draw pixels with
 */
void drawInvertBitmap(int16_t x, int16_t y,
            const uint8_t *bitmap, int16_t w, int16_t h,
            uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if((pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)))==0) {
        tft.drawPixel(x+i, y+j, color);
      }
    }
  }
}

