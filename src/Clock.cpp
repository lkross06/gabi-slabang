#include "Clock.h"

#include <Wire.h>

#include "pins.h"

Clock::Clock() : interrupt_flag(CAS::NONE) {}

bool Clock::begin(){
    if (!Wire.begin(DS3231_SDA, DS3231_SCL)) return false;
    if (!rtc.begin()) return false;

    // adjust by a few seconds to account for compile/build time
    DateTime compileTime = DateTime(F(__DATE__), F(__TIME__));
    rtc.adjust(compileTime.unixtime() + COMPILE_BUILD_TIME_S);

    //For some reason polling the RTC once on init will sync up the clock. I have no idea why
    DateTime now = rtc.now();

    // configure 1Hz Square Wave output on SQW pin
    rtc.writeSqwPinMode(DS3231_SquareWave1Hz);

    return true;
}

bool Clock::update(){
    if (interrupt_flag == CAS::NONE) return false;

    int32_t delta_seconds = 0;
    switch (interrupt_flag){
        case CAS::INC_HOUR:
            delta_seconds = 3600;
            break;
        case CAS::DEC_HOUR:
            delta_seconds = -3600;
            break;
        case CAS::INC_MIN:
            delta_seconds = 60;
            break;
        case CAS::DEC_MIN:
            delta_seconds = -60;
            break;
        default:
            break;
    }

    //reset flag
    interrupt_flag = CAS::NONE;

    //skip an I2C call if we can avoid it
    if (delta_seconds == 0) return true;

    //update RTC over I2C (slow part!!)
    DateTime current = rtc.now();
    rtc.adjust(DateTime(current.unixtime() + delta_seconds));

    return true;
}