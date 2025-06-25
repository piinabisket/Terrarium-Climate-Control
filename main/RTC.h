#ifndef RTC_H
#define RTC_H

#include "externlib.h"

#define MINUTE_INTERVAL 5
#define TIMEOUT_INTERVAL 2

void setupRTC();
void updateClock();
bool checkTimer(int8_t timer, int8_t period, int8_t currTime);
int8_t adjustTime(int8_t time);
#endif