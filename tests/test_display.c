#include <unistd.h>
#include "oled.h"

int main(void) {
  if (oled_init() != 0) return 1;

  oled_draw_text(0, 0, "Zuno Wallet");
  oled_draw_text(0, 1, "Zuno Wallet");
  oled_draw_text(0, 2, "Zuno Wallet");
  oled_draw_text(0, 3, "Zuno Cold Wallet");
  sleep(8);
  oled_clear();

  oled_clear();
  oled_draw_bitmap(0, 0, btc_icon);
  oled_draw_bitmap(20, 0, qr_icon);
  oled_draw_bitmap(40, 0, padlock_locked_icon);
  oled_draw_bitmap(60, 0, padlock_unlocked_icon);
  oled_draw_bitmap(80, 0, key_icon);
  oled_draw_bitmap(100, 0, cog_icon);
  oled_draw_bitmap(0, 2, home_icon);
  oled_draw_bitmap(20, 2, plus_icon);
  oled_draw_bitmap(40, 2, minus_icon);
  oled_draw_bitmap(60, 2, refresh_icon);
  oled_draw_bitmap(80, 2, arrow_left_icon);
  oled_draw_bitmap(100, 2, arrow_right_icon);
  sleep(8);
  oled_clear();

  oled_draw_number(0, 0, 0);
  oled_draw_number(20, 0, 1);
  oled_draw_number(40, 0, 2);
  oled_draw_number(60, 0, 3);
  oled_draw_number(80, 0, 4);
  oled_draw_number(100, 0, 5);
  oled_draw_number(0, 2, 6);
  oled_draw_number(20, 2, 7);
  oled_draw_number(40, 2, 6);
  oled_draw_number(60, 2, 8);
  oled_draw_number(80, 2, 9);
  sleep(8);
  oled_clear();

  oled_close();

  return 0;
}
