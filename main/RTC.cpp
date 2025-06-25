#include "main.h"
#include "RTC.h"
/*
 *  @brief  Initialize RTC, setup timer and current time variables.
 */
void setupRTC(){
  RTC.begin();
  updateClock();  
  timer1 = adjustTime(humidIntervalSetpoint + second);
  timer2 = adjustTime(MINUTE_INTERVAL + minute);
  timeoutTimer = adjustTime(TIMEOUT_INTERVAL + minute);
  // RTC.setHourMode(CLOCK_H12);
  // RTC.setMeridiem(HOUR_PM);
  // RTC.setYear(2025);
  // RTC.setMonth(6);
  // RTC.setDay(19);
  // RTC.setHours(5);
  // RTC.setMinutes(48);
  // RTC.setSeconds(0);
}

/*
 *  @brief  Update time variables to reflect current time.
 */
void updateClock(){
  second = RTC.getSeconds();
  minute = RTC.getMinutes();
  hour = RTC.getHours();
}

/*
 *  @brief  Checks if given timer has triggered yet.
 *  @params timer: Which timer to check, the next cycle for period to reach.
 *          period: Interval between timer triggers, e.g. 5 minutes.
 *          currTime: Current time provided by RTC; hour, minute, second.
 *  @returns  true: timer has been triggered.
 *            false: timer has not been triggered yet.
 */
bool checkTimer(int8_t timer, int8_t period, int8_t currTime){
  if(currTime == timer){
    timer = adjustTime(period + currTime);
    return true;
    #ifdef SERIAL
      Serial.print("Alarm!");
    #endif
  }
  return false;
}

/*
 *  @brief  Handles edge case for time rollover;
 *          If current time is 50 seconds, and period is 20 seconds,
 *          time will be adjusted to 10 seconds.
 */
int8_t adjustTime(int8_t time){
  if(time >= 60)  return time - 60;
  return time;
}

// void lightHandler(){

// }