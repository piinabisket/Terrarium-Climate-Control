   //////////////////////////////////////////////
  //        2.8" TOUCH SCREEN DEMO            //
 //                                          //
//           http://www.educ8s.tv           //
/////////////////////////////////////////////

//#include <Adafruit_TFTLCD.h> 
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>    
#include <TouchScreen.h>

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
#define TEMP_DOWN_Y 66
#define TEMP_UP_X 265
#define TEMP_UP_Y 66
#define BUTTON_H 30
#define BUTTON_W 35
#define HUM_DOWN_X 220
#define HUM_DOWN_Y 108
#define HUM_UP_X 265
#define HUM_UP_Y 108

#define NEXT_X 265
#define NEXT_Y 190
#define PREV_X 20
#define PREV_Y 190

MCUFRIEND_kbv tft;
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
//GFXcanvas1 canvas(38, 38);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int period = 20;
unsigned long time_now = 0;
TSPoint p;
int humidSetpoint = 72;      //setpoints for system to adjust to
int tempSetpoint = 72;
bool tempButtonState = true;    //flag to indicate if button has been pressed
bool humidButtonState = true;
bool configButtonState = true;
int configScreen = 1;

void setup() {
  Serial.begin(9600);
  Serial.print("Starting...");
  tft.reset();
  tft.begin(tft.readID());
  tft.setRotation(3);
  Serial.begin(9600);
  Serial.println();
  Serial.print("reading id....0x");
  delay(500);
  Serial.println(tft.readID(), HEX);
  tft.fillScreen(BLACK);

}

void loop() 
{
  
  time_now = millis();  //get current time
  // Delay between measurements.
  
  while(millis() < time_now + period){
    TSPoint p = ts.getPoint();  //Get touch point
    if (p.z > ts.pressureThreshhold) {

      p.x = map(p.x, TS_MINX, TS_MAXX, 0, 320);
      p.y = map(p.y, TS_MINY, TS_MAXY, 0, 240);
      #ifdef SERIAL
      Serial.print("X = "); Serial.print(p.x);
      Serial.print("\tY = "); Serial.print(p.y);
      Serial.print("\n");
      #endif
      //Next config screen button
      if(p.x>NEXT_X && p.x<NEXT_X + BUTTON_W && p.y>NEXT_Y && p.y<NEXT_Y + BUTTON_H){
        if(!configButtonState){
          if(configScreen == 3){
            configScreen = 1;
          }
          else{
            configScreen++;
          }
        }
        configButtonState = true;
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.fillRect(NEXT_X - 2, NEXT_Y - 2, BUTTON_W + 3, BUTTON_H + 2, BLACK);
        tft.fillRect(NEXT_X, NEXT_Y + 4, BUTTON_W, BUTTON_H + 2, WHITE);
        tft.fillRect(NEXT_X + 1, NEXT_Y + 5, BUTTON_W - 2, BUTTON_H - 6, BLACK);
        tft.drawTriangle(NEXT_X+14,NEXT_Y + 10, NEXT_X+14,NEXT_Y + 22, 
                            NEXT_X+22,NEXT_Y + 16, WHITE);
      }
      //Prev config screen button
      else if(p.x>PREV_X && p.x<PREV_X + BUTTON_W && p.y>PREV_Y && p.y<PREV_Y + BUTTON_H){
        if(!configButtonState){
          if(configScreen == 1){
            configScreen = 3;
          }
          else{
            configScreen--;
          }
        }
        configButtonState = true;
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.fillRect(PREV_X - 2, PREV_Y - 2, BUTTON_W + 3, BUTTON_H + 2, BLACK);
        tft.fillRect(PREV_X, PREV_Y + 4, BUTTON_W, BUTTON_H + 2, WHITE);
        tft.fillRect(PREV_X + 1, PREV_Y + 5, BUTTON_W - 2, BUTTON_H - 6, BLACK);
        tft.drawTriangle(PREV_X+19,PREV_Y + 10, PREV_X+19,PREV_Y + 22, 
                            PREV_X+11,PREV_Y + 16, WHITE);
      }
      if(configScreen == 1){
        //Temp Setpoint DOWN button pressed
        if(p.x>TEMP_DOWN_X && p.x<TEMP_DOWN_X + BUTTON_W && p.y>TEMP_DOWN_Y && p.y<TEMP_DOWN_Y + BUTTON_H){
          if(!tempButtonState){
            tempSetpoint--;
          }
          tempButtonState = true;
          pinMode(XM, OUTPUT);
          pinMode(YP, OUTPUT);
          tft.fillRect(TEMP_DOWN_X - 2, TEMP_DOWN_Y - 2, BUTTON_W + 3, BUTTON_H + 2, BLACK);   //Temp Button Down ON State
          tft.fillRect(TEMP_DOWN_X, TEMP_DOWN_Y + 4, BUTTON_W, BUTTON_H + 2, WHITE);
          tft.fillRect(TEMP_DOWN_X + 1, TEMP_DOWN_Y + 5, BUTTON_W - 2, BUTTON_H - 6, BLACK);
          tft.drawTriangle(TEMP_DOWN_X+8,TEMP_DOWN_Y + 14, (TEMP_DOWN_X+BUTTON_W)-10,TEMP_DOWN_Y + 14, 
                          TEMP_DOWN_X+17,TEMP_DOWN_Y + 23, WHITE);
        }
        //Temp Setpoint UP button pressed
        else if(p.x>TEMP_UP_X && p.x<TEMP_UP_X + BUTTON_W && p.y>TEMP_UP_Y && p.y<TEMP_UP_Y + BUTTON_H){
          if(!tempButtonState){
            tempSetpoint++;
          }
          tempButtonState = true;
          pinMode(XM, OUTPUT);
          pinMode(YP, OUTPUT);
          tft.fillRect(TEMP_UP_X - 2, TEMP_UP_Y - 2, BUTTON_W + 3, BUTTON_H + 2, BLACK);   //Temp Button Up ON State
          tft.fillRect(TEMP_UP_X, TEMP_UP_Y + 4, BUTTON_W, BUTTON_H + 2, WHITE);
          tft.fillRect(TEMP_UP_X + 1, TEMP_UP_Y + 5, BUTTON_W - 2, BUTTON_H -6, BLACK);
          tft.drawTriangle(TEMP_UP_X+8,TEMP_UP_Y+23, (TEMP_UP_X+BUTTON_W)-10,TEMP_UP_Y+23, TEMP_UP_X+17,TEMP_UP_Y+14, WHITE);
        } 
        //Humidity UP button pressed
        else if(p.x>HUM_UP_X && p.x<HUM_UP_X + BUTTON_W && p.y>HUM_UP_Y && p.y<HUM_UP_Y + BUTTON_H){
          if(!humidButtonState){
            if(humidSetpoint < 100){
              humidSetpoint++;
            }
          }
          humidButtonState = true;
          pinMode(XM, OUTPUT);
          pinMode(YP, OUTPUT);
          tft.fillRect(HUM_UP_X - 2, HUM_UP_Y - 2, BUTTON_W + 3, BUTTON_H + 2, BLACK);   //Temp Button Up ON State
          tft.fillRect(HUM_UP_X, HUM_UP_Y + 4, BUTTON_W, BUTTON_H + 2, WHITE);
          tft.fillRect(HUM_UP_X + 1, HUM_UP_Y + 5, BUTTON_W - 2, BUTTON_H -6, BLACK);
          tft.drawTriangle(HUM_UP_X+8,HUM_UP_Y+23, (HUM_UP_X+BUTTON_W)-10,HUM_UP_Y+23, HUM_UP_X+17,HUM_UP_Y+14, WHITE);
        } 
        //Humidity DOWN button pressed
        else if(p.x>HUM_DOWN_X && p.x<HUM_DOWN_X + BUTTON_W && p.y>HUM_DOWN_Y && p.y<HUM_DOWN_Y + BUTTON_H){
          if(!humidButtonState){
            if(humidSetpoint > 0){
              humidSetpoint--;
            }
          }
          humidButtonState = true;
          pinMode(XM, OUTPUT);
          pinMode(YP, OUTPUT);
          tft.fillRect(HUM_DOWN_X - 2, HUM_DOWN_Y - 2, BUTTON_W + 3, BUTTON_H + 2, BLACK);   //Temp Button Up ON State
          tft.fillRect(HUM_DOWN_X, HUM_DOWN_Y + 4, BUTTON_W, BUTTON_H + 2, WHITE);
          tft.fillRect(HUM_DOWN_X + 1, HUM_DOWN_Y + 5, BUTTON_W - 2, BUTTON_H -6, BLACK);
          tft.drawTriangle(HUM_DOWN_X+8,HUM_DOWN_Y + 14, (HUM_DOWN_X+BUTTON_W)-10,HUM_DOWN_Y + 14, 
                          HUM_DOWN_X+17,HUM_DOWN_Y + 23, WHITE);
        } 
      }
      else if(configScreen == 2){
      }
    }
  }
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //Draw Header
  //tft.fillScreen(0);
  tft.setCursor(6, 6);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  tft.print("Config");
  tft.setCursor(125,12);
  tft.setTextSize(2);
  tft.print(configScreen);
  tft.print("/3");
  tft.drawFastHLine(0, 34, 320, WHITE);
  tft.drawRect(0,0,320,240,WHITE);
  if(configButtonState){
    tft.fillScreen(BLACK);
    drawButtons();
    tft.fillRect(NEXT_X - 2, NEXT_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);   //NEXT Button OFF
    tft.fillRect(NEXT_X, NEXT_Y, BUTTON_W, BUTTON_H, WHITE);
    tft.fillTriangle(NEXT_X+14,NEXT_Y + 8, NEXT_X+14,NEXT_Y + 20, 
                            NEXT_X+22,NEXT_Y + 14, BLACK);
    tft.fillRect(PREV_X - 2, PREV_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);   //PREV Button OFF
    tft.fillRect(PREV_X, PREV_Y, BUTTON_W, BUTTON_H, WHITE);
    tft.fillTriangle(PREV_X+19,PREV_Y + 8, PREV_X+19,PREV_Y + 20, 
                            PREV_X+11,PREV_Y + 14, BLACK);
  }
  configButtonState = false;
  //Temperature and humidity setpoint screen
  if(configScreen == 1){
    tft.setTextSize(2);
    tft.setCursor(6, 44);
    tft.println("Setpoints:");
    tft.setCursor(24, 74);
    tft.print("Temp.     ");
    tft.print(tempSetpoint);
    tft.write(0xF7);
    tft.println("F ");
    tft.setCursor(24, 116);
    tft.print("Humidity  ");
    tft.print(humidSetpoint);
    tft.println("% ");
    if(tempButtonState){
      tft.fillRect(TEMP_DOWN_X - 2, TEMP_DOWN_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);   //Temp Down Button OFF State
      tft.fillRect(TEMP_DOWN_X, TEMP_DOWN_Y, BUTTON_W, BUTTON_H, WHITE);
      tft.fillTriangle(TEMP_DOWN_X+8,TEMP_DOWN_Y+10, (TEMP_DOWN_X+35)-10,TEMP_DOWN_Y+10, TEMP_DOWN_X+17,TEMP_DOWN_Y+19, BLACK);
      tft.fillRect(TEMP_UP_X - 2, TEMP_UP_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);
      tft.fillRect(TEMP_UP_X, TEMP_UP_Y, BUTTON_W, BUTTON_H, WHITE);                       //Temp Up Button OFF State
      tft.fillTriangle(TEMP_UP_X+8,TEMP_UP_Y+19, (TEMP_UP_X+BUTTON_W)-10,TEMP_UP_Y+19, TEMP_UP_X+17,TEMP_UP_Y+10, BLACK);
    }
    if(humidButtonState){
      tft.fillRect(HUM_DOWN_X - 2, HUM_DOWN_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);   //Humidity Down Button OFF State
      tft.fillRect(HUM_DOWN_X, HUM_DOWN_Y, BUTTON_W, BUTTON_H, WHITE);
      tft.fillTriangle(HUM_DOWN_X+8,HUM_DOWN_Y+10, (HUM_DOWN_X+35)-10,HUM_DOWN_Y+10, HUM_DOWN_X+17,HUM_DOWN_Y+19, BLACK);
      tft.fillRect(HUM_UP_X - 2, HUM_UP_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);
      tft.fillRect(HUM_UP_X, HUM_UP_Y, BUTTON_W, BUTTON_H, WHITE);                       //Humidity Up Button OFF State
      tft.fillTriangle(HUM_UP_X+8,HUM_UP_Y+19, (HUM_UP_X+BUTTON_W)-10,HUM_UP_Y+19, HUM_UP_X+17,HUM_UP_Y+10, BLACK);
    }
    humidButtonState = false;
    tempButtonState = false;
  }
}

void drawButtons(){
  humidButtonState = true;
  tempButtonState = true;
  configButtonState = true;
}


