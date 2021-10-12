//"hello world" test
#include <U8g2lib.h>
//https://www.jianshu.com/p/ba490b3da904

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.clearBuffer();                   // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB14_tr);   // choose a suitable font
  u8g2.drawStr(0,20,"Hello World!");    // write something to the internal memory
  u8g2.sendBuffer();                    // transfer internal memory to the display
  delay(1000);
}