#include "Clock.h"

#include <Wire.h>

#include "pins.h"

Clock::Clock() : interrupt_flag(ClockSignal::NONE) {}

bool Clock::begin(){
    if (!Wire.begin(DS3231_SDA, DS3231_SCL)) return false;
    if (!rtc.begin()) return false;

    // adjust by a few seconds to account for compile/build time
    DateTime compileTime = DateTime(F(__DATE__), F(__TIME__));
    rtc.adjust(DateTime(compileTime.unixtime() + COMPILE_BUILD_TIME_S));

    //TODO: for some reason the clock still doesn't adjust sometimes
    Serial.printf("At compile time: %02u:%02u:%02u %s\n", compileTime.twelveHour(), compileTime.minute(), compileTime.second(), (compileTime.isPM())? "pm" : "am");

    DateTime now = rtc.now();
    Serial.printf("Clock thinks the time is %02u:%02u:%02u %s\n", now.twelveHour(), now.minute(), now.second(), (now.isPM())? "pm" : "am");

    // configure 1Hz Square Wave output on SQW pin
    rtc.writeSqwPinMode(DS3231_SquareWave1Hz);

    return true;
}

bool Clock::update(){
    if (interrupt_flag == ClockSignal::NONE) return false;

    int32_t delta_seconds = 0;
    switch (interrupt_flag){
        case ClockSignal::INC_HOUR:
            delta_seconds = 3600;
            break;
        case ClockSignal::DEC_HOUR:
            delta_seconds = -3600;
            break;
        case ClockSignal::INC_MIN:
            delta_seconds = 60;
            break;
        case ClockSignal::DEC_MIN:
            delta_seconds = -60;
            break;
        default:
            break;
    }

    //reset flag
    interrupt_flag = ClockSignal::NONE;

    //skip an I2C call if we can avoid it
    if (delta_seconds == 0) return true;

    //update RTC over I2C (slow part!!)
    DateTime current = rtc.now();
    rtc.adjust(DateTime(current.unixtime() + delta_seconds));

    return true;
}