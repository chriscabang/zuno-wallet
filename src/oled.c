#include "oled.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 32
#define OLED_ADDR 0x3C
#define I2C_DEV "/dev/i2c-1"

static int i2c_fd = -1;

// Send command (control byte 0x00)
static void oled_cmd(uint8_t cmd) {
  uint8_t buffer[2] = {0x00, cmd};
  write(i2c_fd, buffer, 2);
}

// Send data (control byte 0x40)
static void oled_data(const uint8_t* data, size_t len) {
  uint8_t buffer[len + 1];
  buffer[0] = 0x40;
  memcpy(&buffer[1], data, len);
  write(i2c_fd, buffer, len + 1);
}

// :TODO: Parameterized i2c bus/address
int oled_init(void) {
  i2c_fd = open(I2C_DEV, O_RDWR);
  if (i2c_fd < 0) {
    perror("open i2c");
    return -1;
  }
  if (ioctl(i2c_fd, I2C_SLAVE, OLED_ADDR) < 0) {
    perror("ioctl i2c");
    return -1;
  }

  // Init sequence (from Adafruit + datasheet)
  const uint8_t init_seq[] = {
    0xAE, 0x20, 0x00, 0xB0,
    0xC8, 0x00, 0x10, 0x40,
    0x81, 0x7F, 0xA1, 0xA6,
    0xA8, 0x1F, 0xD3, 0x00,
    0xD5, 0x80, 0xD9, 0xF1,
    0xDA, 0x02, 0xDB, 0x40,
    0x8D, 0x14, 0xAF
  };
  for (size_t i = 0; i < sizeof(init_seq); ++i) {
    oled_cmd(init_seq[i]);
  }

  oled_clear();
  return 0;
}

void oled_clear(void) {
  for (uint8_t page = 0; page < 4; ++page) {
    oled_cmd(0xB0 + page);      // page address
    oled_cmd(0x00);             // lower column
    oled_cmd(0x10);             // upper column
    uint8_t zeros[OLED_WIDTH] = {0};
    oled_data(zeros, OLED_WIDTH);
  }
}

void oled_draw_text(uint8_t col, uint8_t row, const char* text) {
  if (row > 3 || col >= OLED_WIDTH) return;

  oled_cmd(0xB0 + row);
  oled_cmd(0x00 + (col & 0x0F));        // Lower nibble
  oled_cmd(0x10 + ((col >> 4) & 0x0F)); // Upper nibble

  while (*text && col < OLED_WIDTH - 6) {
    char c = *text++;
    if (c < 32 || c > 127) c = '?';
    oled_data(font_5x8[c - 32], 5);  // 5-byte glyph
    uint8_t space = 0x00;
    oled_data(&space, 1);            // 1-pixel space
    col += 6;
  }
}

// Lookup table
static const uint8_t* DIGIT_BITMAPS[10] = {
  digit_0, digit_1, digit_2, digit_3, digit_4,
  digit_5, digit_6, digit_7, digit_8, digit_9
};

// Draws a number (e.g. 314) using oled_draw_bitmap()
void oled_draw_number(uint8_t col, uint8_t row, int number) {
  char buf[12];
  snprintf(buf, sizeof(buf), "%d", number);

  for (size_t i = 0; i < strlen(buf); ++i) {
    char c = buf[i];
    if (c >= '0' && c <= '9') {
      uint8_t digit = c - '0';
      oled_draw_bitmap(col + i * 18, row, DIGIT_BITMAPS[digit]);
    }
  }
}

void oled_draw_bitmap(uint8_t col, uint8_t row, const uint8_t* bitmap) {
  if (col > 112 || row > 2) return;

  for (uint8_t page = 0; page <2; ++page) {
    oled_cmd(0xB0 + row + page);
    oled_cmd(0x00 + (col & 0x0F));
    oled_cmd(0x10 + ((col >> 4) & 0x0F));
//    oled_data(&bitmap[page * 16], 16);


    uint8_t line[16] = { 0 };
    // for (uint8_t i = 0; i < 16; ++i) {
    //   line[i] = bitmap[i * 2 + page];
    // }
    for (uint8_t x = 0; x < 16; ++x) {
      uint8_t byte = 0;
      for (uint8_t bit = 0; bit < 8; ++bit) {
        uint8_t y = page * 8 + bit;
        uint8_t row_hi = bitmap[y * 2];
        uint8_t row_lo = bitmap[y * 2 + 1];
        uint16_t row_bits = (row_hi << 8) | row_lo;
        if (row_bits & (1 << (15 -x))) {
          byte |= (1 << bit);
        }
      }
      line[x] = byte;
    }
    oled_data(line, 16);

  }
}

void oled_close(void) {
  if (i2c_fd >= 0) close(i2c_fd);
  i2c_fd = -1;
}
