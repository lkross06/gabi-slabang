#pragma once

#include <Arduino.h>
#include <RTClib.h>

#define AM 0
#define PM 1

#define COMPILE_BUILD_TIME_S    8U

/* CLOCK ASYNC SIGNAL TYPES */
enum class CAS : uint8_t {
    NONE        = 0,
    SQW         = 1,    //one second has passed
    INC_HOUR    = 2,
    DEC_HOUR    = 3,
    INC_MIN     = 4,
    DEC_MIN     = 5
};

class Clock {
public:
    Clock();

    bool begin();
    bool update();

    inline uint8_t hour() { return rtc.now().twelveHour(); }
    inline uint8_t minute() { return rtc.now().minute(); }
    inline uint8_t second() { return rtc.now().second(); }
    inline bool time_of_day() { return (rtc.now().hour() > 12)? PM : AM; }

    volatile CAS interrupt_flag;

private:
    RTC_DS3231 rtc; //source of truth!!
};
