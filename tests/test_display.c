#include <unistd.h>
#include "oled.h"
#include "icons_16x16.h"

int main(void) {
  if (oled_init() != 0) return 1;

  oled_clear();
  oled_draw_bitmap(0, 0, btc_icon);
//  oled_draw_bitmap(20, 0, arrow_up_icon);
//  oled_draw_bitmap(40, 0, qr_icon);
  oled_draw_text(0, 0, "Zuno Wallet");

  sleep(5);

  oled_clear();
  oled_close();

  return 0;
}
