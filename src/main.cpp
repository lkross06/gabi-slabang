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

void IRAM_ATTR onSecond();

void setup() {
    Serial.begin(115200);
    delay(1000);

    if (!rtc.begin()) {Serial.println("failure"); while(1); };

    // global interrupt handler for 1Hz Square Wave from DS3231 RTC
    pinMode(DS3231_SQW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(DS3231_SQW), onSecond, FALLING);
}

void loop() {
    if (rtc.interrupt_flag){
        rtc.interrupt_flag = false;
        rtc.update();
        Serial.printf("%02d:%02d:%02d %s\n", rtc.hour(), rtc.minute(), rtc.second(), (rtc.time_of_day() == AM)? "AM" : "PM");
    }
    delay(10);
}

void IRAM_ATTR onSecond(){
    rtc.interrupt_flag = true;
}