#ifndef RELAY_H
#define RELAY_H
#include "Arduino.h"

class Relay {

   public:
      Relay(uint8_t pin);
      void begin(void);
      void on(void);
      void off(void);
      bool status(void);
   private:
      uint8_t _pin;
      bool _status;     //ON = true, OFF = false
};
#endif