/**
* 演示 TCPServer 功能
* 打开 TCP 调试助手 模拟 TCP client 请求
*/

#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 2, 14);

#include <ESP8266WiFi.h>
//定义最多多少个 client 可以连接本 server(一般不要超过 4 个)
#define MAX_SRV_CLIENTS 1
//以下三个定义为调试定义
#define DebugBegin(baud_rate) Serial.begin(baud_rate)
#define DebugPrintln(message) Serial.println(message)
#define DebugPrint(message) Serial.print(message)
// const char* ssid = "GreenIoT";
// const char* password = "******** ";

const char *ssid = "gerogerori";
const char *password = "****";

//创建 server 端口号是 2020
// TCPServer server(2020);
WiFiServer server(2020);
//管理 clients
WiFiClient serverClients[MAX_SRV_CLIENTS];
void setup()
{
  DebugBegin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  DebugPrint("\nConnecting to ");
  DebugPrintln(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20)
  {
    delay(500);
  }
  if (i == 21)
  {
    DebugPrint("Could not connect to");
    DebugPrintln(ssid);
    while (1)
    {
      delay(500);
    }
  }
  //启动 server
  server.begin();
  //关闭小包合并包功能，不会延时发送数据
  server.setNoDelay(true);
  DebugPrint("Ready! Use 'telnet ");
  DebugPrint(WiFi.localIP());
  DebugPrintln(" 2020' to connect");

  display.init();
}

// void displayStr(){

// }

void loop()
{

  display.clear();

  // 物联网技术与应用
  // 12
  uint8_t i;
  //检测是否有新的 client 请求进来
  if (server.hasClient())
  {
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      //释放旧无效或者断开的 client
      if (!serverClients[i] || !serverClients[i].connected())
      {
        if (serverClients[i])
        {
          serverClients[i].stop();
        }
        //分配最新的 client
        serverClients[i] = server.available();
        DebugPrint("New client: ");
        DebugPrint(i);

        break;
      }
    }
    //当达到最大连接数 无法释放无效的 client，需要拒绝连接
    if (i == MAX_SRV_CLIENTS)
    {
      WiFiClient serverClient = server.available();
      serverClient.stop();
      DebugPrintln("Connection rejected ");
    }
  }
  //检测 client 发过来的数据
  for (i = 0; i < MAX_SRV_CLIENTS; i++)
  {
    if (serverClients[i] && serverClients[i].connected())
    {
      if (serverClients[i].available())
      {
        String str = "";
        //get data from the telnet client and push it to the UART
        while (serverClients[i].available())
        {
          char ch = (char)serverClients[i].read();
          str += ch;
          //发送到串口调试器
          Serial.write(ch);
          //  Serial.write(serverClients[i].read());
        }

        display.drawString(0, 0, str);
        display.display();
      }
    }
  }
  if (Serial.available())
  {
    //把串口调试器发过来的数据 发送给 client
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      if (serverClients[i] && serverClients[i].connected())
      {
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }
}
