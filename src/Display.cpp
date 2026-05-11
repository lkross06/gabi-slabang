#include "Display.h"

#include "splash.h"
#include "utils.h"

namespace {
    #define TEXT_SIZE_1_WIDTH   5U
    #define TEXT_SIZE_1_HEIGHT  8U
    #define TEXT_SIZE_2_WIDTH   2U * TEXT_SIZE_1_WIDTH
    #define TEXT_SIZE_2_HEIGHT  2U * TEXT_SIZE_1_HEIGHT
    #define TEXT_SIZE_3_WIDTH   3U * TEXT_SIZE_1_WIDTH
    #define TEXT_SIZE_3_HEIGHT  3U * TEXT_SIZE_1_HEIGHT
}

Display::Display() : 
    SSD1306(SCREEN_WIDTH_PX,
        SCREEN_HEIGHT_PX,
        &SPI,
        SSD1306_DC,
        SSD1306_RST,
        SSD1306_CS
    ),
    SSD1306_init(false),
    current_page(DisplayPage::NONE),
    last_clock_render({0, 0, 0, false}),
    last_counter_render({0, 0, 0})
{};

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
    SSD1306.drawBitmap(0, 0, loading_data, loading_width, loading_height, SSD1306_WHITE);
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

bool Display::renderClock(uint8_t hour, uint8_t min, uint8_t sec, bool isPM){
    if (!SSD1306_init) return false;
    if (!should_renderClock(hour, min, sec, isPM)) return false;
    reset();

    SSD1306.setTextColor(SSD1306_WHITE);
    SSD1306.setTextSize(3);

    SSD1306.setCursor((SCREEN_WIDTH_PX - (TEXT_SIZE_3_WIDTH * 7)) / 2, (SCREEN_HEIGHT_PX - TEXT_SIZE_3_HEIGHT) / 2);
    SSD1306.printf("%02u:%02u", hour, min);

    SSD1306.setTextSize(1);
    SSD1306.print((isPM)? " pm" : " am");

    SSD1306.display();

    return true;
}

bool Display::renderCounter(uint32_t count, uint8_t month, uint8_t day){
    if (!SSD1306_init) return false;
    if (!should_renderCounter(count, month, day)) return false;
    reset();

    SSD1306.setTextColor(SSD1306_WHITE);

    SSD1306.setCursor(22, 22);
    if (count > 1){
        SSD1306.drawBitmap(0, 0, ndays_data, ndays_width, ndays_height, SSD1306_WHITE);

        SSD1306.setTextSize(2);
        SSD1306.printf("%3u", count);
    } else if (count == 1) {
        SSD1306.drawBitmap(0, 0, tomorrow_data, tomorrow_width, tomorrow_height, SSD1306_WHITE);
    } else {
        SSD1306.drawBitmap(0, 0, today_data, today_width, today_height, SSD1306_WHITE);
    }

    char formatted_date[MAX_FORMATTED_MD_SIZE];
    size_t n = formatMD(month, day, formatted_date);

    if (n > 0) {
        SSD1306.setCursor(0, SCREEN_HEIGHT_PX - 8);
        SSD1306.setTextSize(1);
        SSD1306.print(formatted_date);
    }

    SSD1306.display();

    return true;
}

void Display::reset(){
    SSD1306.clearDisplay();
    SSD1306.setCursor(0, 0);
}

bool Display::should_renderClock(uint8_t hour, uint8_t min, uint8_t sec, bool isPM){
    bool changed = (hour != last_clock_render.hour || min != last_clock_render.min || sec != last_clock_render.sec || isPM != last_clock_render.isPM);

    if (current_page == DisplayPage::CLOCK && !changed) return false; 

    current_page = DisplayPage::CLOCK;
    last_clock_render.hour = hour;
    last_clock_render.min = min;
    last_clock_render.sec = sec;
    last_clock_render.isPM = isPM;

    return true;
}

bool Display::should_renderCounter(uint32_t count, uint8_t month, uint8_t day){
    bool changed = (count != last_counter_render.count || month != last_counter_render.month || day != last_counter_render.day);

    if (current_page == DisplayPage::COUNTER && !changed) return false;

    current_page = DisplayPage::COUNTER;
    last_counter_render.count = count;
    last_counter_render.month = month;
    last_counter_render.day = day;

    return true;
}