#pragma once

#include <Arduino.h>
#include <RTClib.h>

#define AM 0
#define PM 1

/* Estimate of the time between the computer last updates the RTC's time ref, and when RTC starts counting */
#define COMPILE_BUILD_TIME_S    7U

/* CLOCK ASYNC SIGNAL TYPES */
enum class ClockSignal : uint8_t {
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

    /* Initialize RTC module, start counting with 1Hz square wave interrupts */
    bool begin();
    /* Looks at the interrupt flag set by the global interrupt handler, then adjusts the RTC internal clock. Return true if any changes are made */
    bool update();

    inline DateTime now() { return rtc.now(); }

    volatile ClockSignal interrupt_flag;
private:
    RTC_DS3231 rtc; //source of truth!!
};
