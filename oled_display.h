#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <string>
#include <cstdint>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>

// SSD1306 OLEDÏÔÊ¾ÆÁ³£Á¿
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_PAGES (OLED_HEIGHT / 8)

// SSD1306ÃüÁî
#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

// ÏÔÊ¾ÃüÁî
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_PAGEADDR 0x22
#define SSD1306_COLUMNADDR 0x21

class OLEDDisplay
{
public:
    OLEDDisplay(const std::string &i2c_device, uint8_t address = 0x3C);
    ~OLEDDisplay();

    void initialize();
    void clear();
    void display();
    void setPixel(int x, int y, bool on);
    void drawChar(int x, int y, char c, bool invert = false);
    void drawString(int x, int y, const std::string &text, bool invert = false);
    void drawRect(int x, int y, int width, int height, bool fill = false);
    void drawLine(int x0, int y0, int x1, int y1);

private:
    int fd;
    uint8_t address;
    uint8_t buffer[OLED_WIDTH * OLED_PAGES];

    void sendCommand(uint8_t command);
    void sendData(const uint8_t *data, size_t size);
    void setAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
};

#endif // OLED_DISPLAY_H