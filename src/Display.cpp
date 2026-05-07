#include "Display.h"

#include "splash.h"

Display::Display() : 
    SSD1306(SCREEN_WIDTH_PX,
        SCREEN_HEIGHT_PX,
        &SPI,
        SSD1306_DC,
        SSD1306_RST,
        SSD1306_CS
    ) {
    SSD1306_init = false;
};

bool Display::begin(){
    SPI.begin(SSD1306_CLK, SSD1306_MISO, SSD1306_MOSI, SSD1306_CS);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    SSD1306_init = SSD1306.begin(SSD1306_SWITCHCAPVCC);
    if (!SSD1306_init) return false;

    return true;
}

bool Display::renderLoadingSequence(uint16_t wait_ms, uint8_t steps_px){
    if (!SSD1306_init) return false;
    if (wait_ms < 7000) return false;   //too fast might fry our OLED with continuous renders
    reset();

    uint32_t delta_ms = wait_ms / steps_px;

    // render initial image
    SSD1306.drawBitmap(0, 0, loading_splash_data, loading_splash_width, loading_splash_height, SSD1306_WHITE);
    SSD1306.display();

    delay(2000);

    // render loading bar
    for (uint8_t i = 0; i < steps_px; i++){
        SSD1306.drawLine(64 + i, 53, 64 + i, 63, SSD1306_WHITE);
        SSD1306.display();
        delay(delta_ms);
    }

    delay(1000);

    return true;
}

bool Display::renderClock(uint8_t hour, uint8_t min){
    if (!SSD1306_init) return false;
    reset();

    SSD1306.setCursor(SCREEN_WIDTH_PX / 2, SCREEN_HEIGHT_PX / 2);
    SSD1306.setTextColor(SSD1306_WHITE);
    SSD1306.setTextSize(1);
    SSD1306.printf("%02u:%02u", hour, min);
    SSD1306.display();

    return true;
}

bool Display::renderCounter(uint32_t count){
    if (!SSD1306_init) return false;
    reset();

    SSD1306.setCursor(SCREEN_WIDTH_PX / 2, SCREEN_HEIGHT_PX / 2);
    SSD1306.setTextColor(SSD1306_WHITE);
    SSD1306.setTextSize(1);
    SSD1306.printf("%u", count);
    SSD1306.display();

    return true;
}

void Display::reset(){
    SSD1306.clearDisplay();
    SSD1306.setCursor(0, 0);
}