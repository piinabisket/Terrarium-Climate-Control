#ifndef LCDDRIVER_H
#define LCDDRIVER_H

#include <stdint.h>
#include <Arduino.h>

class LCD_Driver{
   public:
   LCD_Driver(int rst, int cs, int dc, int rd, int wr, int d[7]);
   void initPins();
   void writeData8(uint8_t data);
   void writeCommand(uint8_t cmd);
   void writeData(uint8_t data);
   void reset();
   void begin();
   private:
   uint8_t RST, CS, DC, RD, WR;
   uint8_t D[7];
};

#endif