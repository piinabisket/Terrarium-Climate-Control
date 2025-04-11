   //////////////////////////////////////////////
  //        2.8" TOUCH SCREEN DEMO            //
 //                                          //
//           http://www.educ8s.tv           //
/////////////////////////////////////////////

#include <Adafruit_TFTLCD.h> 
#include <Adafruit_GFX.h>    
#include <TouchScreen.h>

#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 
#define LCD_RESET A4 

#define TS_MINX 100
#define TS_MINY 100
#define TS_MAXX 960
#define TS_MAXY 930

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

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

boolean buttonEnabled = true;
int period = 20;
unsigned long time_now = 0;
TSPoint p;
int humiditySetpoint = 72;
int tempSetpoint = 72;
bool tempButton = true;

void setup() {
  Serial.begin(9600);
  Serial.print("Starting...");
  
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);
  tft.fillScreen(BLACK);

}

void loop() 
{
  time_now = millis();  //get current time
  // Delay between measurements.
  
  while(millis() < time_now + period){
    TSPoint p = ts.getPoint();  //Get touch point
    if (p.z > ts.pressureThreshhold) {
      Serial.print("X = "); Serial.print(p.x);
      Serial.print("\tY = "); Serial.print(p.y);
      Serial.print("\n");
      p.x = map(p.x, TS_MINX, TS_MAXX, 0, 320);
      p.y = map(p.y, TS_MINY, TS_MAXY, 0, 240);

      if(p.x>225 && p.x<260 && p.y>66 && p.y<96){
        if(tempButton){
          tempSetpoint--;
        }
        tempButton = false;
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.fillRect(223, 64, 38, 32, BLACK);
        tft.fillRect(225, 70, 35, 32, WHITE);
        tft.fillRect(226, 71, 33, 24, BLACK);
      }
      else if(p.x>275 && p.x<310 && p.y>66 && p.y<96){
        if(tempButton){
          tempSetpoint++;
        }
        tempButton = false;
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        tft.fillRect(273, 64, 38, 32, BLACK);
        tft.fillRect(275, 70, 35, 32, WHITE);
        tft.fillRect(276, 71, 33, 24, BLACK);
      } 
    }
  }
  tempButton = true;
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //Draw Header
  tft.setCursor(6, 6);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  tft.print("Config");
  tft.drawFastHLine(0, 34, 320, WHITE);

  tft.setTextSize(2);
  tft.setCursor(6, 44);
  tft.println("Setpoints:");
  tft.setCursor(24, 74);
  tft.print("Temperature ");
  tft.print(tempSetpoint);
  tft.write(0xF7);
  tft.println("F");
  tft.setCursor(24, 104);
  tft.print("Humidity    ");
  tft.print(humiditySetpoint);
  tft.println("%");
  tft.drawRect(0,0,320,240,WHITE);

  tft.fillRect(223, 64, 38, 38, BLACK);
  tft.fillRect(273, 64, 38, 38, BLACK);
  tft.fillRect(225, 66, 35, 30, WHITE);
  tft.fillRect(275, 66, 35, 30, WHITE);
  delay(10);  
}

