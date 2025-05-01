#include "lcd-driver.h"

/* INI9341 LCD display control pins*/
#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 
#define LCD_RESET A4 

LCD_Driver::LCD_Driver(int rst, int cs, int dc, int rd, int wr, int d[8]){
   RST = rst;
   CS = cs;
   DC = dc;
   RD = rd;
   WR = wr;
   for(int i = 0; i < 8; i++){
      D[i] = d[i];
   }
   initPins();
}

/*
 * @brief Initialize all pins
 */
void LCD_Driver::initPins() {
   pinMode(CS, OUTPUT);
   pinMode(RST, OUTPUT);
   pinMode(DC, OUTPUT);
   pinMode(WR, OUTPUT);
   pinMode(RD, OUTPUT);
   pinMode(D[0], OUTPUT);
   pinMode(D[1], OUTPUT);
   pinMode(D[2], OUTPUT);
   pinMode(D[3], OUTPUT);
   pinMode(D[4], OUTPUT);
   pinMode(D[5], OUTPUT);
   pinMode(D[6], OUTPUT);
   pinMode(D[7], OUTPUT);
}

/*
 *  @brief  Write data to parallel data pins
 *  @params data: 8-bit data to be written to display
 */
void LCD_Driver::writeData8(uint8_t data) {
   digitalWrite(D[0], (data & 0x01));
   digitalWrite(D[1], (data & 0x02) >> 1);
   digitalWrite(D[2], (data & 0x04) >> 2);
   digitalWrite(D[3], (data & 0x08) >> 3);
   digitalWrite(D[4], (data & 0x10) >> 4);
   digitalWrite(D[5], (data & 0x20) >> 5);
   digitalWrite(D[6], (data & 0x40) >> 6);
   digitalWrite(D[7], (data & 0x80) >> 7);
   
   // Send the write signal
   digitalWrite(WR, LOW);
   delayMicroseconds(1);
   digitalWrite(WR, HIGH);
}

/*
 *  @brief  Send command to display
 *  @params cmd: 8-bit command to be executed by display
 */
void LCD_Driver::writeCommand(uint8_t cmd) {
   digitalWrite(DC, LOW);  // Command mode
   digitalWrite(CS, LOW);
   writeData(cmd);
   digitalWrite(CS, HIGH);
}

/*
 *  @brief  Write data to display GRAM
 *  @params data: 8-bit data to be displayed
 */
void LCD_Driver::writeData(uint8_t data) {
   digitalWrite(DC, HIGH); // Data mode
   digitalWrite(CS, LOW);
   writeData8(data);           
   digitalWrite(CS, HIGH); 
}

/*
 *  @brief  Reset display
 */
void LCD_Driver::reset() {
   digitalWrite(RST, LOW);
   delay(50);
   digitalWrite(RST, HIGH);
   delay(50);
}
/*
void LCD_Driver::drawPixel(int16_t x, int16_t y, uint16_t color) {
   if(x < 0 || y < 0 || x >= 320 || y >= 240){return;}
   setAddrWindow(x, y, x, y);
   writeCommand
}
*/
/*
 *  @brief  Initialize ILI9341 TFT to spec
 */
void LCD_Driver::begin() {
   initPins();
   
   reset();
   
   writeCommand(0x01); // Software reset
   delay(150);         // Wait for reset to complete

   writeCommand(0x28); // Display OFF
   delay(100);

   //Interface control
   writeCommand(0xF6);
   writeData(0x01);
   writeData(0x01);
   writeData(0x00);

      
   // Power control
   writeCommand(0xCB);   //Power control A
   writeData(0x39);
   writeData(0x2C);
   writeData(0x00);
   writeData(0x34);
   writeData(0x02);

   writeCommand(0xCF);   //Power control B
   writeData(0x00);
   writeData(0x81);
   writeData(0x30);

   writeCommand(0xED);   //Power On Sequence
   writeData(0x64);
   writeData(0x03);
   writeData(0x12);
   writeData(0x81);

   //Driver timing control
   writeCommand(0xE8);   //Driver timing control A
   writeData(0x85);
   writeData(0x10);
   writeData(0x78);

   writeCommand(0xEA);   //Driver timing control B
   writeData(0x00);
   writeData(0x00);

   // Initialize the display settings (power control, frame rate, etc.)
   writeCommand(0xF7);   //Pump ratio
   writeData(0x20);   

   writeCommand(0xB0);   //RGB Signal
   writeData(0x00);   

   writeCommand(0xB1);  //Frame control
   writeData(0x00);
   writeData(0x1B);

   writeCommand(0xC0);  // Power control 1
   writeData(0x21);

   writeCommand(0xC1);  // Power control 2
   writeData(0x11);

   writeCommand(0xC5);  // VCOM control 1
   writeData(0x3F);
   writeData(0x3C);

   writeCommand(0xC7);  // VCOM control 2
   writeData(0xB5);

   writeCommand(0xF2);  //Enable 3G
   writeData(0x00);

   writeCommand(0x26);  //Enable 3G
   writeData(0x01);

   // Display ON
   writeCommand(0x29);

   // Clear screen
   writeCommand(0x2C);
}
