#include <Arduino.h>

#include "Display.h"

enum class MODE : bool {
  CLOCK = 0,
  COUNTER = 1
};

Display display;

void setup() {
  Serial.begin(115200);
  delay(1000);

  while (!display.begin()) { delay(500); }

  Serial.println("Setup complete");

  display.renderLoadingSequence();
}

void loop() {

}