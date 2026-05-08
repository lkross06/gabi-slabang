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

    enum class DisplayPage : uint8_t {
        NONE    = 0,
        LOADING = 1,
        CLOCK   = 2,
        COUNTER = 3
    };

    Display();

    /* Start SPI and OLED display, allows renders and reset to do work when called */
    bool begin();

    /* Renders a bitmap and plays loading bar animation on startup. Just looks cool lol */
    bool renderLoadingSequence(uint16_t wait_ms = DEFAULT_LOADING_WAIT_MS, uint8_t steps_px = DEFAULT_LOADING_STEPS_PX);
    /* Renders the clock screen with a given time */
    bool renderClock(uint8_t hour, uint8_t min, uint8_t sec, bool isPM);
    /* Renders the day counter with a set count */
    bool renderCounter(uint32_t count, uint8_t month, uint8_t day);
    
    /* Clears display and resets mode to default, renders default screen */
    void reset();
private:
    // OLED 128x64 display module
    Adafruit_SSD1306 SSD1306;
    bool SSD1306_init;

    //Keep track of last display state so we don't keep re-rendering the same thing
    DisplayPage current_page;
    
    struct {
        uint8_t hour, min, sec;
        bool isPM;
    } last_clock_render;

    struct {
        uint32_t count;
        uint8_t month, day;
    } last_counter_render;

    bool should_renderClock(uint8_t hour, uint8_t min, uint8_t sec, bool isPM);
    bool should_renderCounter(uint32_t count, uint8_t month, uint8_t day);
};