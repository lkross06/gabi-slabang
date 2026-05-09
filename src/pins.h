#pragma once

/* SSD1306 OLED 128x64 Display via SPI */
#define SSD1306_CLK         18      // labeled SCK
#define SSD1306_MISO        19      // this NEVER gets used, just map to an unused GPIO for SPI init
#define SSD1306_MOSI        23      // labeled SDA
#define SSD1306_RST         15      // labeled RES
#define SSD1306_DC          2
#define SSD1306_CS          5

/* DS3231 Real-Time Clock (RTC) via I2C */
#define DS3231_SDA          21
#define DS3231_SCL          22
#define DS3231_SQW          4       // interrupts on INPUT_PULLUP at set rate

/* User-controlled tactile buttons */
#define BUTTON_SWITCHSTATE  32
#define BUTTON_INCHOUR      33
#define BUTTON_DECHOUR      25
#define BUTTON_INCMIN       26
#define BUTTON_DECMIN       27
#define BUTTON_INCCOUNT     14
#define BUTTON_DECCOUNT     13