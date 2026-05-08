#pragma once

#include <RTClib.h>

/* COUNTER ASYNC SIGNAL TYPES */
enum class CounterSignal : uint8_t {
    NONE        = 0,
    INC_COUNT   = 1,
    DEC_COUNT   = 2
};

class Counter {
public:
    Counter();

    /* Initialize DoA to today */
    bool begin();
    /* Looks at the interrupt flag set by the global interrupt handler, then adjusts DoA. Return true if any changes are made */
    bool update();
    /* Calculate number of days between today and DoA, set DoA = today if in the past */
    uint32_t num_days_between(DateTime today);

    inline uint8_t doa_month() { return day_of_arrival.month(); }
    inline uint8_t doa_day() { return day_of_arrival.day(); }

    volatile CounterSignal interrupt_flag;
private:
    //we want to keep a counter of how many days between this day and now
    DateTime day_of_arrival;    //should always be in the future, and will auto-update with rtc.now()
};