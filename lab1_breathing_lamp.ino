
// mqp

//void setup() {
//  Serial.begin(115200);
//  // put your setup code here, to run once:
//
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//FlasgMode_t ideMode=ESP.getFlashChipMode();
//String coreVersion=ESP.getCoreVersion();
//Serial.print(F("Ardui  :"));
//Serial.println(coreVersion);
//Serial.printf("Flasg rel  Id : %08X \n",ESP.getFlashChipId());
//delay(1000);
//}

//void setup()
//{
//  Serial.begin(9600);
//}
//
//void loop()
//{
//  FlashMode_t ideMode = ESP.getFlashChipMode();
//  String coreVersion = ESP.getCoreVersion();
//  Serial.print(F("Arduino Core For ESP8266 Version"));
//  Serial.println(coreVersion);
//  Serial.printf("Flash real id: %08X\n\n", ESP.getFlashChipId());
//  delay(1000);
//}
//

//  怎么同时控制两个
// void fadeOn(unsigned int time,int increament){
//   for(byte value=0;value<255;value+=increament){
//             analogWrite(ledPin, value);
//             delay(time/(255/increament));
//    }

// }

// void fadeOff(unsigned int time,int decreament){
//   for(byte value=255;value>0;value-=decreament){
//         analogWrite(ledPin,value);
//             delay(time/(255/decreament));

//     }

// }
// ————————————————
// 版权声明：本文为CSDN博主「漠宸离若」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https://blog.csdn.net/weixin_43349929/article/details/105223330

void setup()
{
  // Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  //   digitalWrite(LED_BUILTIN,HIGH);
  //     delay(1000);
  // digitalWrite(LED_BUILTIN,LOW);
  //     delay(1000);

  // int val=700;
  int val = 1024;
  // 这个 不显示 1024
  // int rangeFrom=0;
  // int rangeTo=2048;
  // 一下子亮了

  int rangeFrom = 500;
  int rangeTo = 1023;
  // 在闪电
  // 500--1023  有的
  int ledRangeFrom = 500;
  // int ledPwm=100;

  // int ledPwm = 0;
  // // int ledPwm2=800;
  // int ledPwm2 = 2000;

  //  int ledPwm = 0;
  // // int ledPwm2=800;
  // int ledPwm2 = 1024;

  int ledPwm = 1;
  // int ledPwm2=800;
  // int ledPwm2 = 1023;
  // 这里要比 top 小 1 ，不然 进入不了循环
  int ledPwm2 = 2047;

  // int led1Top=2048;
  // int led1Top=1024;
  // int led2Top=1024;

  int led1Top = 2048;
  int led2Top = 2048;

  int cnt = 500;
  // for (int i = rangeFrom; i < rangeTo; i+=30)
  for (int i = 0; i < cnt && ledPwm > 0 && ledPwm < led1Top && ledPwm2 > 0 && ledPwm2 < led2Top; i++)
  {
    /* code */
    // analogWrite(LED_BUILTIN,val);
    //   analogWrite(4,2048-val);
    // analogWrite(LED_BUILTIN,i);
    analogWrite(LED_BUILTIN, ledPwm);
    //  ledPwm+=20;
    // ledPwm+=10;
    // ledPwm += 4;
    ledPwm += (led1Top * 1.0 / cnt);
    // analogWrite(4,2048-i);
    //  analogWrite(4,1023-i);
    analogWrite(4, ledPwm2);
    // ledPwm2-=10;
    // ledPwm2 -= 4;
    ledPwm2 -= (led2Top * 1.0 / cnt);
    // 没有到最不亮的地步
    // 2000-500 不行的  1024--1500 是没有的
    //  delay(250);
    // delay(30);
    delay(10);
    // delay(125);
    // 0  600
    // 1 400
    // 2 300
    // 3 200
    //  val-=100;
    // val-=200;
  }

  // ledPwm=800;
  // ledPwm2=100;

  // ledPwm = 2047;
  // ledPwm2 = 1;

  //   ledPwm = 1024;
  // ledPwm2 = 1;

  // ledPwm = 1023;
  ledPwm = 2047;
  ledPwm2 = 1;
  // 知道了 他的范围是这些 0--2048

  for (int i = 0; i < cnt && ledPwm > 0 && ledPwm < led1Top && ledPwm2 > 0 && ledPwm2 < led2Top; i++)
  {
    /* code */
    // analogWrite(LED_BUILTIN,val);
    //   analogWrite(4,2048-val);
    // analogWrite(LED_BUILTIN,i);
    analogWrite(LED_BUILTIN, ledPwm);
    //  这里到了 最暗
    //  ledPwm+=20;
    // ledPwm+=10;
    //  ledPwm-=10;
    // ledPwm -= 4;
    ledPwm -= (led1Top * 1.0 / cnt);
    // analogWrite(4,2048-i);
    //  analogWrite(4,1023-i);
    analogWrite(4, ledPwm2);
    // ledPwm2-=10;
    //  ledPwm2+=10;
    // ledPwm2 += 4;
    ledPwm2 += (led2Top * 1.0 / cnt);
    // 2000-500 不行的  1024--1500 是没有的
    //  delay(250);
    // delay(30);
    // 有点慢了
    delay(10);
    // delay(125);
    // 0  600
    // 1 400
    // 2 300
    // 3 200
    //  val-=100;
    // val-=200;
  }

  // for (int i = rangeTo; i > rangeFrom; i-=30)
  // {
  //   /* code */
  //   // analogWrite(LED_BUILTIN,val);
  //   //   analogWrite(4,2048-val);
  //       analogWrite(LED_BUILTIN,i);
  //     // analogWrite(4,2048-i);
  //      analogWrite(4,1023-i);
  // //  delay(250);
  //   delay(30);
  //   // delay(125);
  //   // 0  600
  //   // 1 400
  //   // 2 300
  //   // 3 200
  // //  val-=100;
  //   // val-=200;
  // }

  // 这个还行吧
  // analogWrite(LED_BUILTIN,2048);
  //  delay(600);
  //  analogWrite(LED_BUILTIN,1848);
  // delay(400);
  //  analogWrite(LED_BUILTIN,1500);
  // delay(300);
  //  analogWrite(LED_BUILTIN,1100);
  // delay(200);

  // analogWrite(LED_BUILTIN,1024);
  //  delay(600);
  //  analogWrite(LED_BUILTIN,824);
  // delay(400);
  //  analogWrite(LED_BUILTIN,624);
  // delay(300);
  //  analogWrite(LED_BUILTIN,424);
  // delay(200);

  // analogWrite(LED_BUILTIN,624);
  //  delay(600);
  //  analogWrite(LED_BUILTIN,824);
  // delay(400);
  //  analogWrite(LED_BUILTIN,1024);
  // delay(300);
  //  analogWrite(LED_BUILTIN,1100);
  // delay(200);

  // analogWrite(LED_BUILTIN,1300);
  //  delay(600);
  //  analogWrite(LED_BUILTIN,1500);
  // delay(400);
  //  analogWrite(LED_BUILTIN,1600);
  // delay(300);
  //  analogWrite(LED_BUILTIN,2000);
  // delay(200);

  // .......

  // analogWrite(4,2048);
  //  delay(600);
  //  analogWrite(4,1848);
  // delay(400);
  //  analogWrite(4,1500);
  // delay(300);
  //  analogWrite(4,1100);
  // delay(200);

  // analogWrite(4,1024);
  //  delay(600);
  //  analogWrite(4,824);
  // delay(400);
  //  analogWrite(4,624);
  // delay(300);
  //  analogWrite(4,424);
  // delay(200);
  // .... 和 红的 一起的

  // FlashMode_t ideMode = ESP.getFlashChipMode();
  // String coreVersion = ESP.getCoreVersion();
  // Serial.print(F("Arduino Core For ESP8266 Version"));
  // Serial.println(coreVersion);
  // Serial.printf("Flash real id: %08X\n\n", ESP.getFlashChipId());
  // delay(1000);
}
