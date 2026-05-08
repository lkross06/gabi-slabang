#include "Counter.h"

Counter::Counter() : interrupt_flag(CounterSignal::NONE) {}

bool Counter::begin(){
    DateTime compileTime(F(__DATE__), F(__TIME__));

    //we want DoA to always point at midnight so that we can tell if the deadline has passed
    day_of_arrival = DateTime(
        compileTime.year(),
        compileTime.month(),
        compileTime.day()
        //omitted time field defaults to midnight
    );

    return true; //always returns true
}

bool Counter::update(){
    if (interrupt_flag == CounterSignal::NONE) return false;

    int32_t delta_seconds = 0;
    switch(interrupt_flag){
        case CounterSignal::INC_COUNT:
            delta_seconds = 86400;
            break;
        case CounterSignal::DEC_COUNT:
            delta_seconds = -86400;
            break;
        default:
            break;
    }

    //reset flag
    interrupt_flag = CounterSignal::NONE;
    if (delta_seconds == 0) return true;

    day_of_arrival = day_of_arrival + TimeSpan(delta_seconds);

    return true;
}

uint32_t Counter::num_days_between(DateTime today){
    int32_t diff = (day_of_arrival - today).totalseconds();

    if (diff <= 0){
        day_of_arrival = DateTime(
            today.year(),
            today.month(),
            today.day()
            //omitted time field defaults to midnight
        );
    }

    return (diff + 43200) / 86400; //number of days, rounded
}