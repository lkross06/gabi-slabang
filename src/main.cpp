#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "pins.h"

Adafruit_SSD1306 SSD1306(SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX, &SPI, SSD1306_DC, SSD1306_RST, SSD1306_CS);

uint64_t i;
volatile uint64_t i_lastinterrupt;

void IRAM_ATTR handler();

void setup() {
  Serial.begin(115200);
  delay(1000);

  i = 0;
  i_lastinterrupt = 0;

  pinMode(41, INPUT_PULLUP); //GPIO41 --> button --> GND
  attachInterrupt(digitalPinToInterrupt(41), handler, FALLING);

  SPI.begin(SSD1306_CLK, SSD1306_MISO, SSD1306_MOSI, SSD1306_CS);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  while (!SSD1306.begin(SSD1306_SWITCHCAPVCC)) { delay(100); }

  SSD1306.clearDisplay();
  SSD1306.setTextSize(1);
  SSD1306.setTextColor(SSD1306_WHITE);

  Serial.println("Setup complete");
}

void loop() {
  SSD1306.clearDisplay();
  SSD1306.setCursor(0,0);
  SSD1306.printf("%d", i);
  SSD1306.display();
  delay(50);
  i++;
}

void IRAM_ATTR handler() {
  if (i - i_lastinterrupt < 10) return;

  Serial.printf("Button pressed! i=%d\n", i);
  i_lastinterrupt = i;
}