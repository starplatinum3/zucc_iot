//#include<SSD1306Wire.h>
#include "SSD1306Wire.h"
//SSD1306Wire 字体大小

SSD1306Wire display(0x3c,2,14);

//https://zhuanlan.zhihu.com/p/111482208

//温度 引脚是 5
//串口先输出试试
void setup()
{

//   Serial.begin(115200);
   display.init();
}

//可能是叠加在一起了


void loop()
{
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");
display.display();

//  FlashMode_t ideMode = ESP.getFlashChipMode();
//  String coreVersion = ESP.getCoreVersion();
//  Serial.print(F("Arduino Core For ESP8266 Version"));
//  Serial.println(coreVersion);
//  Serial.printf("Flash real id: %08X\n", ESP.getFlashChipId());
//  delay(1000);
}
