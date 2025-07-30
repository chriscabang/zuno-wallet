#ifndef OLED_H
#define OLED_H

#include <stdint.h>

// Init the OLED on /dev/i2c-1 (addr 0x3C)
int oled_init(void);

// Clear the display
void oled_clear(void);

// Display ASCII string at column (0-127) and row (0-3)
void oled_draw_text(uint8_t col, uint8_t row, const char* text);
void oled_draw_bitmap(uint8_t col, uint8_t row, const  uint8_t* bitmap);

// Optional cleanup
void oled_close(void);

#endif//OLED_H
