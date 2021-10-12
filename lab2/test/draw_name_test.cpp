#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/14, /* data=*/2, /* reset=*/U8X8_PIN_NONE);
void setup(void) {
  u8g2.begin();
  u8g2.enableUTF8Print();		// enable UTF8 support for the Arduino print() function
}
 
//  https://blog.csdn.net/MENGHUANBEIKE/article/details/75666266?fps=1&locationNum=7
void loop(void) {
  u8g2.setFont(u8g2_font_unifont_t_chinese2);  // use chinese2 for all the glyphs of "你好世界"
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  // u8g2.print("Hello World!");
  // u8g2.setCursor(0, 40);
  u8g2.print("缪奇鹏");		// Chinese "Hello World" 
   u8g2.setCursor(0, 40);
   u8g2.print("31901077");
  u8g2.sendBuffer();
  
  delay(1000);
}