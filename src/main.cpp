#include <Arduino.h>
#include <EasyButton.h>
#include <Wire.h>
#include <RTClib.h>

#include "pins.h"
#include "Display.h"
#include "Clock.h"

enum class STATE : bool {
  CLOCK = 0,
  COUNTER = 1
};

Clock rtc;
Display display;
volatile STATE state;

EasyButton button_switchState(BUTTON_SWITCHSTATE);
EasyButton button_incHour(BUTTON_INCHOUR);
EasyButton button_decHour(BUTTON_DECHOUR);
EasyButton button_incMin(BUTTON_INCMIN);
EasyButton button_decMin(BUTTON_DECMIN);

void IRAM_ATTR handle_sqw() { rtc.interrupt_flag = CAS::SQW; }
void IRAM_ATTR handle_incHour() { rtc.interrupt_flag = CAS::INC_HOUR; }
void IRAM_ATTR handle_decHour() { rtc.interrupt_flag = CAS::DEC_HOUR; }
void IRAM_ATTR handle_incMin() { rtc.interrupt_flag = CAS::INC_MIN; }
void IRAM_ATTR handle_decMin() { rtc.interrupt_flag = CAS::DEC_MIN; }
void IRAM_ATTR handle_switchState() { state = (state == STATE::CLOCK)? STATE::COUNTER : STATE::CLOCK; }

void setup() {
    Serial.begin(115200);
    delay(1000);

    //NOTE: start RTC before display so that clock counts through the loading sequence
    if (!rtc.begin()) { Serial.println("failure"); while(1); };
    if (!display.begin()) { Serial.println("failure"); while(1); };

    // global interrupt handler for 1Hz Square Wave from DS3231 RTC
    pinMode(DS3231_SQW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(DS3231_SQW), handle_sqw, FALLING);

    button_switchState.begin();
    button_incHour.begin();
    button_decHour.begin();
    button_incMin.begin();
    button_decMin.begin();

    button_switchState.onPressed(handle_switchState);
    button_incHour.onPressed(handle_incHour);
    button_decHour.onPressed(handle_decHour);
    button_incMin.onPressed(handle_incMin);
    button_decMin.onPressed(handle_decMin);

    state = STATE::CLOCK;

    Serial.println("Setup complete");
    display.renderLoadingSequence();
}

void loop() {
    DateTime now = rtc.now();

    if(rtc.update()){
        Serial.printf("%02u:%02u:%02u %s\n", now.twelveHour(), now.minute(), now.second(), (now.isPM())? "PM" : "AM");
    }

    button_switchState.read();
    button_incHour.read();
    button_decHour.read();
    button_incMin.read();
    button_decMin.read();
  
    if (state == STATE::CLOCK){
        display.renderClock(now.twelveHour(), now.minute(), now.second(), now.isPM());
    } else {
        display.renderCounter(67);
    }

    delay(5);
}