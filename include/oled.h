#ifndef OLED_H
#define OLED_H

#include <stdint.h>

#include "font_5x8.h"
#include "icons_16x16.h"
#include "digit_16x16.h"

int oled_init(void);
void oled_clear(void);
void oled_close(void);

// Display ASCII string at column (0-127) and row (0-3)
void oled_draw_text(uint8_t col, uint8_t row, const char* text);
void oled_draw_number(uint8_t col, uint8_t row, int number);
void oled_draw_bitmap(uint8_t col, uint8_t row, const  uint8_t* bitmap);

#endif//OLED_H
