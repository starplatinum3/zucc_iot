/**
* 功能描述：OTA 之 Arduino IDE 更新 V1.0 版本代码
*
*/
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
//调试定义
#define DebugBegin(baud_rate) Serial.begin(baud_rate)
#define DebugPrintln(message) Serial.println(message)
#define DebugPrint(message) Serial.print(message)
#define DebugPrintF(...) Serial.printf(__VA_ARGS__)
#define CodeVersion "CodeVersion V2.0"
// const char *ssid = "xxxx";      //填上 wifi 账号
// const char *password = "xxxxx"; //填上 wifi 密码

const char *ssid = "esp-mqp";      //填上 wifi 账号
const char *password = "xxxxx"; //填上 wifi 密码

const int pin_r = 12;
const int pin_g = 13;
const int pin_b = 15;


void rgbSetup(){
  pinMode(pin_r, OUTPUT);
  pinMode(pin_g, OUTPUT);
  pinMode(pin_b, OUTPUT);
}
void drawRgb(int r, int g, int b)
{
  analogWrite(pin_r, r);
  analogWrite(pin_g, g);
  analogWrite(pin_b, b);
}


void setup()
{
  DebugBegin(115200);
  DebugPrintln("Booting Sketch....");
  DebugPrintln(CodeVersion);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  rgbSetup();
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    DebugPrintln("Connection Failed! Rebooting...");
    delay(5000);
    //重启 ESP8266 模块
    ESP.restart();
  }
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA.onStart([]() {
    String type;
    //判断一下 OTA 内容
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else
    { // U_SPIFFS
      type = "filesystem";
    }
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    DebugPrintln("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    DebugPrintln("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    DebugPrintF("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    DebugPrintF("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      DebugPrintln("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      DebugPrintln("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      DebugPrintln("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      DebugPrintln("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      DebugPrintln("End Failed");
    }
  });
  ArduinoOTA.begin();
  DebugPrintln("Ready");
  DebugPrint("IP address: ");
  DebugPrintln(WiFi.localIP());
  drawRgb(111,111,111);
}
void loop()
{
  ArduinoOTA.handle();
}