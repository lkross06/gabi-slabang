#include "Clock.h"

#include <Wire.h>

#include "pins.h"

Clock::Clock() : interrupt_flag(false) {}

bool Clock::begin(){
    if (!Wire.begin(DS3231_SDA, DS3231_SCL)) return false;
    if (!rtc.begin()) return false;

    // adjust by ~7s for compile time
    const auto compile_time_s = 7U;
    DateTime compileTime = DateTime(F(__DATE__), F(__TIME__));
    rtc.adjust(compileTime.unixtime() + compile_time_s);

    // configure 1Hz Square Wave output on SQW pin
    rtc.writeSqwPinMode(DS3231_SquareWave1Hz);

    return true;
}

void Clock::update(){
    now = rtc.now();
}