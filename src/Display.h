#pragma once

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "pins.h"

#define SCREEN_WIDTH_PX     128U
#define SCREEN_HEIGHT_PX    64U

#define DEFAULT_LOADING_WAIT_MS     10000U
#define DEFAULT_LOADING_STEPS_PX    64U

class Display {
public:
    Display();

    /** Start SPI and OLED display, allows renders and reset to do work when called */
    bool begin();

    bool renderLoadingSequence(uint16_t wait_ms = DEFAULT_LOADING_WAIT_MS, uint8_t steps_px = DEFAULT_LOADING_STEPS_PX);
    bool renderClock(uint8_t hour, uint8_t min, uint8_t sec, bool isPM);
    bool renderCounter(uint32_t count);
    
    /** Clears display and resets mode to default, renders default screen */
    void reset();
private:
    // OLED 128x64 display module
    Adafruit_SSD1306 SSD1306;
    bool SSD1306_init;
};