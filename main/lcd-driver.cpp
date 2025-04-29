
/* INI9341 LCD display control pins*/
#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 
#define LCD_RESET A4 

/* Parallel data pins */
#define TFT_D0    22
#define TFT_D1    23
#define TFT_D2    24
#define TFT_D3    25
#define TFT_D4    26
#define TFT_D5    27
#define TFT_D6    28
#define TFT_D7    29


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

LCD_Driver::LCD_Driver(int rst, int cs, int dc, int rd, int wr, int d[7]){
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
   writeData(data);           
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
 *  @brief  Initialize ILI9341 TFT to spec
 */
void LCD_Driver::begin() {
  initPins();
  
  reset();
  
  writeCommand(0x01); // Software reset
  delay(150);         // Wait for reset to complete

  writeCommand(0x28); // Display OFF
  delay(100);


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
  writeData(0xA9);   
  writeData(0x51);
  writeData(0x2C);
  writeData(0x82);

  writeCommand(0xC0);  // Power control 1
  writeData(0x21); //left off

  writeCommand(0xC1);  // Power control 2
  writeData(0x10);

  writeCommand(0xC5);  // VCM control
  writeData(0x3E);
  writeData(0x28);

  writeCommand(0xC7);  // VCM control 2
  writeData(0x86);

  // Display ON
  writeCommand(0x29);

  // Clear screen
  writeCommand(0x2C);
}
