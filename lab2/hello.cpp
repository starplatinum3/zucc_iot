#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 2, 14);
void setup()
{
  display.init();
}

void loop()
{

  display.clear();

  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hello, mqp!");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 20, "Hello, mqp!");
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 40, "Hello, mqp!");

  display.display();

  // 题目有两种版本
  // 因为没有  ArialMT_Plain_12 所以就用  用10号，16号，24号字号
  // 用10号，16号，24号字号显示三行字符“Hello, ZUCC!” (将***替换为自己的姓名缩写)。

  // 1. 用10号，12号，16号字号显示三行字符“Hello, ***!” (将***替换为自己的姓名缩写)
}
