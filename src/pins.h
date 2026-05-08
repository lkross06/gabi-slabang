#pragma once

/* SSD1306 OLED 128x64 Display via SPI */
#define SSD1306_CLK         12      // labeled SCK
#define SSD1306_MISO        13      // this NEVER gets used, just map to an unused GPIO for SPI init
#define SSD1306_MOSI        11      // labeled SDA
#define SSD1306_RST         9       // labeled RES
#define SSD1306_DC          8
#define SSD1306_CS          10

/* DS3231 Real-Time Clock (RTC) via I2C */
#define DS3231_SDA          41
#define DS3231_SCL          42
#define DS3231_SQW          40      // interrupts on INPUT_PULLUP at set rate