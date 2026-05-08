#include <Arduino.h>
#include <EasyButton.h>
#include <Wire.h>
#include <RTClib.h>

#include "pins.h"
#include "Display.h"
#include "Clock.h"
#include "Counter.h"

enum class STATE : bool {
  CLOCK = 0,
  COUNTER = 1
};

//Initialize RTC, counter, and LED display modules. Keep track of system state
Clock rtc;
Counter counter;
Display display;
volatile STATE state;

//User-controlled tactile buttons, debounced with EasyButton sampling
EasyButton button_switchState(BUTTON_SWITCHSTATE);
EasyButton button_incHour(BUTTON_INCHOUR);
EasyButton button_decHour(BUTTON_DECHOUR);
EasyButton button_incMin(BUTTON_INCMIN);
EasyButton button_decMin(BUTTON_DECMIN);
EasyButton button_incCount(BUTTON_INCCOUNT);
EasyButton button_decCount(BUTTON_DECCOUNT);

// Global interrupt handlers
void IRAM_ATTR handle_sqw() { if(state == STATE::CLOCK) rtc.interrupt_flag = ClockSignal::SQW; }
void IRAM_ATTR handle_incHour() { if(state == STATE::CLOCK) rtc.interrupt_flag = ClockSignal::INC_HOUR; }
void IRAM_ATTR handle_decHour() { if(state == STATE::CLOCK) rtc.interrupt_flag = ClockSignal::DEC_HOUR; }
void IRAM_ATTR handle_incMin() { if(state == STATE::CLOCK) rtc.interrupt_flag = ClockSignal::INC_MIN; }
void IRAM_ATTR handle_decMin() { if(state == STATE::CLOCK) rtc.interrupt_flag = ClockSignal::DEC_MIN; }

void IRAM_ATTR handle_switchState() { state = (state == STATE::CLOCK)? STATE::COUNTER : STATE::CLOCK; }

void IRAM_ATTR handle_incCount() { if(state == STATE::COUNTER) counter.interrupt_flag = CounterSignal::INC_COUNT; }
void IRAM_ATTR handle_decCount() { if(state == STATE::COUNTER) counter.interrupt_flag = CounterSignal::DEC_COUNT; }

void setup() {
    Serial.begin(115200);
    delay(1000);

    //NOTE: start RTC before display so that clock counts through the loading sequence
    if (!rtc.begin()) { Serial.println("failure"); while(1); };
    if (!display.begin()) { Serial.println("failure"); while(1); };
    if (!counter.begin()) { Serial.println("failure"); while(1); };

    // global interrupt handler for 1Hz Square Wave from DS3231 RTC
    pinMode(DS3231_SQW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(DS3231_SQW), handle_sqw, FALLING);

    button_switchState.begin();
    button_incHour.begin();
    button_decHour.begin();
    button_incMin.begin();
    button_decMin.begin();
    button_incCount.begin();
    button_decCount.begin();

    button_switchState.onPressed(handle_switchState);
    button_incHour.onPressed(handle_incHour);
    button_decHour.onPressed(handle_decHour);
    button_incMin.onPressed(handle_incMin);
    button_decMin.onPressed(handle_decMin);
    button_incCount.onPressed(handle_incCount);
    button_decCount.onPressed(handle_decCount);

    state = STATE::CLOCK;

    Serial.println("Setup complete");
    display.renderLoadingSequence();
}

void loop() {
    // poll all peripherals
    button_switchState.read();
    button_incHour.read();
    button_decHour.read();
    button_incMin.read();
    button_decMin.read();
    button_incCount.read();
    button_decCount.read();
    
    rtc.update();
    counter.update();

    if (state == STATE::CLOCK){
        DateTime now = rtc.now();
        display.renderClock(now.twelveHour(), now.minute(), now.second(), now.isPM());
    } else if (state == STATE::COUNTER){
        uint32_t num_days = counter.num_days_between(rtc.now());
        display.renderCounter(num_days, counter.doa_month(), counter.doa_day());
    }
}