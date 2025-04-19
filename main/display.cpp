#include "display.h"

/*
 * Display home screen
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
  tft.print("Hello World");
  tft.drawRect(0,0,320,240,WHITE);
}
/*
 * Display config screen
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
  if(screen == 1){
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
    if(tempButtonState){
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
  //Light and time config screen
  else if(screen == 2){
    tft.setTextSize(2);
    tft.setCursor(6, 49);
    tft.print("Light and Time:");
  }
  //Interval adjustment config screen
  else if(screen == 3){
    tft.setTextSize(2);
    tft.setCursor(6, 49);
    tft.print("Intervals:");
  }
}
/*
 * Simple function to refresh buttons on display by changing each buttons
 * flag
 */
void drawButtons(){
  humidButtonState = true;
  tempButtonState = true;
  configButtonState = true;
  homeButtonState = true;
}
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
void tempUpButtonCallback(){
  tempSetpoint++;
}
void tempDownButtonCallback(){
  tempSetpoint--;
}
void humidUpButtonCallback(){
  humidSetpoint++;
}
void humidDownButtonCallback(){
  humidSetpoint--;
}
void homeButtonCallback(){
  screen = 1;
}
void backButtonCallback(){
  screen = 0;
  if(tempSetpoint != EEPROM.read(TS_MEM)){
    EEPROM.write(TS_MEM, tempSetpoint);
  }
  if(humidSetpoint != EEPROM.read(HS_MEM)){
    EEPROM.write(HS_MEM, humidSetpoint);
  }
}
void nextButtonCallback(){
  if(screen == 3){
    screen = 1;
  }
  else{
    screen++;
  }
}

void prevButtonCallback(){
  if(screen == 1){
    screen = 3;
  }
  else{
    screen--;
  }
}

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

