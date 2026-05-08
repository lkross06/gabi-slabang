#pragma once

#include <Arduino.h>
#include <RTClib.h>

#define AM 0
#define PM 1

class Clock {
public:
    Clock();

    bool begin();
    void update();

    uint8_t hour() { return now.twelveHour(); }
    uint8_t minute() { return now.minute(); }
    uint8_t second() { return now.second(); }
    bool time_of_day() { return (now.hour() > 12)? PM : AM; }

    volatile bool interrupt_flag;

private:
    DateTime now;
    RTC_DS3231 rtc;
};
