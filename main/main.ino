   //////////////////////////////////////////////
  //        2.8" TOUCH SCREEN DEMO            //
 //                                          //
//           http://www.educ8s.tv           //
/////////////////////////////////////////////

//#include <Adafruit_TFTLCD.h> 
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

#define HOME_X 265
#define HOME_Y 5
#define NEXT_X 265
#define NEXT_Y 190
#define PREV_X 20
#define PREV_Y 190

#define BUTTON_H 30
#define BUTTON_W 35

//User settings memoy locations
#define TS_MEM 0
#define HS_MEM 1

MCUFRIEND_kbv tft;
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
//GFXcanvas1 canvas(38, 38);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int period = 20;
unsigned long time_now = 0;
TSPoint p;
int8_t humidSetpoint;      //setpoints for system to adjust to
int8_t tempSetpoint;
bool tempButtonState = true;    //flag to indicate if button has been pressed
bool humidButtonState = true;
bool configButtonState = true;
bool homeButtonState = true;
int configScreen = 1;


const unsigned char backButton [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0xff, 0xf7, 0xff, 0xff, 0xe0, 0xff, 0xe7, 0xff, 0xff, 0xe0, 0xff, 0xc7, 
	0xff, 0xff, 0xe0, 0xff, 0x87, 0xff, 0xff, 0xe0, 0xff, 0x00, 0x01, 0xff, 0xe0, 0xfe, 0x00, 0x00, 
	0x7f, 0xe0, 0xfc, 0x00, 0x00, 0x1f, 0xe0, 0xfe, 0x00, 0x00, 0x0f, 0xe0, 0xff, 0x00, 0x00, 0x0f, 
	0xe0, 0xff, 0x87, 0xfc, 0x07, 0xe0, 0xff, 0xc7, 0xfe, 0x07, 0xe0, 0xff, 0xe7, 0xff, 0x07, 0xe0, 
	0xff, 0xf7, 0xff, 0x07, 0xe0, 0xff, 0xff, 0xff, 0x07, 0xe0, 0xff, 0xff, 0xfe, 0x07, 0xe0, 0xff, 
	0xff, 0xfc, 0x0f, 0xe0, 0xff, 0xff, 0x00, 0x0f, 0xe0, 0xff, 0xff, 0x00, 0x1f, 0xe0, 0xff, 0xff, 
	0x00, 0x1f, 0xe0, 0xff, 0xff, 0x00, 0x7f, 0xe0, 0xff, 0xff, 0x01, 0xff, 0xe0, 0xff, 0xff, 0xff, 
	0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 
	0xe0, 0xff, 0xff, 0xff, 0xff, 0xe0
};

const unsigned char configMenuButton [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0x1f, 0xe0, 0xff, 
	0xff, 0xfe, 0x1f, 0xe0, 0xff, 0xff, 0xfc, 0x7f, 0xe0, 0xff, 0xff, 0xf8, 0xff, 0xe0, 0xff, 0xff, 
	0xf8, 0xf9, 0xe0, 0xff, 0xff, 0xf8, 0xf9, 0xe0, 0xff, 0xff, 0xf8, 0x71, 0xe0, 0xff, 0xff, 0xf0, 
	0x03, 0xe0, 0xff, 0xff, 0xe0, 0x07, 0xe0, 0xff, 0xff, 0xc0, 0x0f, 0xe0, 0xff, 0xff, 0x80, 0xff, 
	0xe0, 0xff, 0xff, 0x01, 0xff, 0xe0, 0xff, 0xfe, 0x03, 0xff, 0xe0, 0xff, 0xfc, 0x07, 0xff, 0xe0, 
	0xff, 0xf8, 0x0f, 0xff, 0xe0, 0xff, 0xf0, 0x1f, 0xff, 0xe0, 0xff, 0x00, 0x3f, 0xff, 0xe0, 0xfe, 
	0x00, 0x7f, 0xff, 0xe0, 0xfc, 0x00, 0xff, 0xff, 0xe0, 0xf8, 0xe1, 0xff, 0xff, 0xe0, 0xf9, 0xf1, 
	0xff, 0xff, 0xe0, 0xf9, 0xf1, 0xff, 0xff, 0xe0, 0xf9, 0xf1, 0xff, 0xff, 0xe0, 0xf8, 0xe3, 0xff, 
	0xff, 0xe0, 0xfc, 0x07, 0xff, 0xff, 0xe0, 0xfe, 0x0f, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 
	0xe0, 0xff, 0xff, 0xff, 0xff, 0xe0
};

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
  //Read setpoints from EEPROM
  tempSetpoint = EEPROM.read(TS_MEM);
  humidSetpoint = EEPROM.read(HS_MEM);
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
      if(configScreen){   //Detect button presses on config screen
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
          tft.drawTriangle(NEXT_X+14, NEXT_Y + 10, NEXT_X+14, NEXT_Y + 22, 
                              NEXT_X+22, NEXT_Y + 16, WHITE);
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
          tft.drawTriangle(PREV_X+19, PREV_Y + 10, PREV_X+19, PREV_Y + 22, 
                              PREV_X+11, PREV_Y + 16, WHITE);
        }
        //Return from config to home button
        else if(p.x>HOME_X && p.x<HOME_X + BUTTON_W && p.y>HOME_Y && p.y<HOME_Y + BUTTON_H){
          if(!configButtonState){
            configScreen = 0;
          }
          if(tempSetpoint != EEPROM.read(TS_MEM)){
            EEPROM.write(TS_MEM, tempSetpoint);
          }
          if(humidSetpoint != EEPROM.read(HS_MEM)){
            EEPROM.write(HS_MEM, humidSetpoint);
          }
          configButtonState = true;
          pinMode(XM, OUTPUT);
          pinMode(YP, OUTPUT);
          tft.fillRect(HOME_X - 2, HOME_Y - 2, BUTTON_W + 3, BUTTON_H + 2, BLACK);
          tft.fillRect(HOME_X, HOME_Y + 4, BUTTON_W, BUTTON_H + 2, WHITE);
          tft.fillRect(HOME_X + 1, HOME_Y + 5, BUTTON_W - 2, BUTTON_H - 6, BLACK);
          tft.drawBitmap(HOME_X, HOME_Y, backButton, 35, 30, BLACK);
          tft.fillScreen(BLACK);
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
      //Home Screen buttons
      else{
        //Home screen to config screen button
        if(p.x>HOME_X && p.x<HOME_X + BUTTON_W && p.y>HOME_Y && p.y<HOME_Y + BUTTON_H){
          if(!homeButtonState){
            configScreen = 1;
          }
          homeButtonState = true;
          pinMode(XM, OUTPUT);
          pinMode(YP, OUTPUT);
          tft.fillRect(HOME_X - 2, HOME_Y - 2, BUTTON_W + 3, BUTTON_H + 2, BLACK);
          tft.fillRect(HOME_X, HOME_Y + 4, BUTTON_W, BUTTON_H + 2, WHITE);
          tft.fillRect(HOME_X + 1, HOME_Y + 5, BUTTON_W - 2, BUTTON_H - 6, BLACK);
        }
      }
    }
  }
  if(configScreen){
    displayConfig();
  }
  else{
    displayHome();
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
/*
 * Display home screen
 */
void displayHome(){
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  //Animate home page buttons
  if(homeButtonState){
    tft.drawBitmap(HOME_X, HOME_Y, configMenuButton, 35, 30, WHITE);
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
    tft.fillRect(NEXT_X - 2, NEXT_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);   //NEXT Button OFF
    tft.fillRect(NEXT_X, NEXT_Y, BUTTON_W, BUTTON_H, WHITE);
    tft.fillTriangle(NEXT_X+14,NEXT_Y + 8, NEXT_X+14,NEXT_Y + 20, 
                            NEXT_X+22,NEXT_Y + 14, BLACK);

    tft.fillRect(PREV_X - 2, PREV_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);   //PREV Button OFF
    tft.fillRect(PREV_X, PREV_Y, BUTTON_W, BUTTON_H, WHITE);
    tft.fillTriangle(PREV_X+19,PREV_Y + 8, PREV_X+19,PREV_Y + 20, 
                            PREV_X+11,PREV_Y + 14, BLACK);
    
    tft.drawBitmap(HOME_X, HOME_Y, backButton, 35, 30, WHITE);
    // tft.fillRect(HOME_X - 2, HOME_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);   //Home Screen button OFF
    // tft.fillRect(HOME_X, HOME_Y, BUTTON_W, BUTTON_H, WHITE);
    // tft.fillTriangle(HOME_X+19,HOME_Y + 8, HOME_X+19,HOME_Y + 20, 
    //                         HOME_X+11,HOME_Y + 14, BLACK);
  }
  configButtonState = false;
  //Draw Header
  tft.setCursor(6, 8);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  tft.print("Config");
  tft.setCursor(125,12);
  tft.setTextSize(2);
  tft.print(configScreen);
  tft.print("/3");
  tft.drawFastHLine(0, 40, 320, WHITE);
  tft.drawRect(0,0,320,240,WHITE);

  //Temperature and humidity setpoint screen
  if(configScreen == 1){
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
      tft.fillRect(TEMP_DOWN_X - 2, TEMP_DOWN_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);   //Temp Down Button OFF State
      tft.fillRect(TEMP_DOWN_X, TEMP_DOWN_Y, BUTTON_W, BUTTON_H, WHITE);
      tft.fillTriangle(TEMP_DOWN_X+8,TEMP_DOWN_Y+10, (TEMP_DOWN_X+35)-10,TEMP_DOWN_Y+10, TEMP_DOWN_X+17,TEMP_DOWN_Y+19, BLACK);
      tft.fillRect(TEMP_UP_X - 2, TEMP_UP_Y - 2, BUTTON_W + 3, BUTTON_H + 8, BLACK);
      tft.fillRect(TEMP_UP_X, TEMP_UP_Y, BUTTON_W, BUTTON_H, WHITE);                       //Temp Up Button OFF State
      tft.fillTriangle(TEMP_UP_X+8,TEMP_UP_Y+19, (TEMP_UP_X+BUTTON_W)-10,TEMP_UP_Y+19, TEMP_UP_X+17,TEMP_UP_Y+10, BLACK);
    }
    //Animate humidity adjustment buttons
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
  //Light and time config screen
  else if(configScreen == 2){
    tft.setTextSize(2);
    tft.setCursor(6, 49);
    tft.print("Light and Time:");
  }
  //Interval adjustment config screen
  else if(configScreen == 3){
    tft.setTextSize(2);
    tft.setCursor(6, 49);
    tft.print("Intervals:");
  }
}

