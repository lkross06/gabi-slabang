// #include <Arduino.h>
// #include <EasyButton.h>

// #include "Display.h"

// enum class MODE : bool {
//   CLOCK = 0,
//   COUNTER = 1
// };

// Display display;
// volatile MODE currentMode;

// EasyButton button(21);
// void IRAM_ATTR handler();

// void setup() {
//   Serial.begin(115200);
//   delay(1000);

//   currentMode = MODE::CLOCK;

//   while (!display.begin()) { delay(500); }

//   button.begin();
//   button.onPressed(handler);

//   Serial.println("Setup complete");

//   display.renderLoadingSequence();
//   if (currentMode == MODE::CLOCK) { display.renderClock(11, 30); }
//   else { display.renderCounter(67); }
// }

// void loop() {
//   button.read();
  
//   if (currentMode == MODE::CLOCK){
//     display.renderClock(11, 30);
//   } else {
//     display.renderCounter(67);
//   }
// }

// void IRAM_ATTR handler() {
//   currentMode = (currentMode == MODE::CLOCK) ? MODE::COUNTER : MODE::CLOCK;
// }

// #include <Wire.h>
// #include <RTClib.h>

// #include "pins.h"

// RTC_DS3231 rtc;
// volatile bool secondTick = false;

// // ISR: This runs exactly once per second
// void IRAM_ATTR onSecond() {
//     secondTick = true;
// }

#include "Clock.h"
#include "pins.h"

Clock rtc;

/* Handle a tick of the square wave interrupt sent by DS3231 RTC at 1 Hz */
void IRAM_ATTR handle_sqw() { rtc.interrupt_flag = CAS::SQW; }
void IRAM_ATTR handle_incHour() { rtc.interrupt_flag = CAS::INC_HOUR; }
void IRAM_ATTR handle_decHour() { rtc.interrupt_flag = CAS::DEC_HOUR; }
void IRAM_ATTR handle_incMin() { rtc.interrupt_flag = CAS::INC_MIN; }
void IRAM_ATTR handle_decMin() { rtc.interrupt_flag = CAS::DEC_MIN; }

void setup() {
    Serial.begin(115200);
    delay(1000);

    if (!rtc.begin()) {Serial.println("failure"); while(1); };

    // global interrupt handler for 1Hz Square Wave from DS3231 RTC
    pinMode(DS3231_SQW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(DS3231_SQW), handle_sqw, FALLING);
}

void loop() {
    if (rtc.update()){
        Serial.printf("%02d:%02d:%02d %s\n", rtc.hour(), rtc.minute(), rtc.second(), (rtc.time_of_day() == AM)? "AM" : "PM");
    }
    delay(10);
}