#include "relay.h"

/*
 *  @brief  Instantiates a new Relay class
 *  @param  pin
 *          pin number that sensor is connected
 */
Relay::Relay(uint8_t pin){
  _pin = pin;
}

/*
 *  @brief  Set up relay pin
 */
void Relay::begin(void){
  pinMode(_pin, OUTPUT);  //set up pin
  digitalWrite(_pin, HIGH);
  _status = false;
}

/*
 *  @brief  Turns relay on
 */
void Relay::on(void){
  digitalWrite(_pin, LOW);
  _status = true;
}

/*
 *  @brief  Turns relay off
 */
void Relay::off(void){
  digitalWrite(_pin, HIGH);
  _status = false;
}

/*
 *  @brief  Returns status of relay state
 *  @returns  bool
 *            true = relay on; false = relay off
 */
bool Relay::status(void){ return _status; }