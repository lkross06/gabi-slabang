#pragma once

#define SCREEN_WIDTH_PX     128
#define SCREEN_HEIGHT_PX    64

/* SSD1306 OLED 128x64 Display via SPI */
#define SSD1306_CLK         12      // labeled SCK
#define SSD1306_MISO        13      // this NEVER gets used, just map to an unused GPIO for SPI init
#define SSD1306_MOSI        11      // labeled SDA
#define SSD1306_RST         9       // labeled RES
#define SSD1306_DC          8
#define SSD1306_CS          10