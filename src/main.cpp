#include <Arduino.h>
#include <EasyButton.h>

#include "Display.h"

enum class MODE : bool {
  CLOCK = 0,
  COUNTER = 1
};

Display display;
volatile MODE currentMode;

EasyButton button(21);
void IRAM_ATTR handler();

void setup() {
  Serial.begin(115200);
  delay(1000);

  currentMode = MODE::CLOCK;

  while (!display.begin()) { delay(500); }

  button.begin();
  button.onPressed(handler);

  Serial.println("Setup complete");

  display.renderLoadingSequence();
  if (currentMode == MODE::CLOCK) { display.renderClock(11, 30); }
  else { display.renderCounter(67); }
}

void loop() {
  button.read();
  
  if (currentMode == MODE::CLOCK){
    display.renderClock(11, 30);
  } else {
    display.renderCounter(67);
  }
}

void IRAM_ATTR handler() {
  currentMode = (currentMode == MODE::CLOCK) ? MODE::COUNTER : MODE::CLOCK;
}