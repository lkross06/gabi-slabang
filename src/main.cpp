#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "pins.h"
#include "../test/splash.h"

Adafruit_SSD1306 SSD1306(SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX, &SPI, SSD1306_DC, SSD1306_RST, SSD1306_CS);

uint64_t i = 0;
uint64_t TOTAL_MS = 7000;
uint64_t STEPS = 64;

void setup() {
  Serial.begin(115200);
  delay(1000);


  SPI.begin(SSD1306_CLK, SSD1306_MISO, SSD1306_MOSI, SSD1306_CS);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  while (!SSD1306.begin(SSD1306_SWITCHCAPVCC)) { delay(100); }

  SSD1306.clearDisplay();
  SSD1306.drawBitmap(0, 0, splash1_data, splash1_width, splash1_height, SSD1306_WHITE);
  SSD1306.display();

  delay(2000);

  uint32_t delta_ms = TOTAL_MS / STEPS;

  for (; i < STEPS; i++){
    SSD1306.drawLine(64 + i, 53, 64 + i, 63, SSD1306_WHITE);
    SSD1306.display();
    delay(delta_ms);
  }

  delay(1000);
}

void loop() {
  SSD1306.clearDisplay();
  // SSD1306.setCursor(0,0);
  // SSD1306.printf("%d", i);
  // SSD1306.display();
  // delay(50);
  // i++;
}