#include "SSD1306Wire.h"
SSD1306Wire *display;
void setup()
{
  display->init();
}

// 这代码也是可以的

void loop()
{

  display = new SSD1306Wire(0x3c, 2, 14);
  display->clear();
  display->setFont(ArialMT_Plain_10);
  display->drawString(5, 0, "Hello, mqp!");
  display->setFont(ArialMT_Plain_16);
  display->drawString(5, 20, "Hello, mqp!");
  display->setFont(ArialMT_Plain_24);
  display->drawString(5, 40, "Hello, mqp!");
  display->display();
  delay(2000);
}
