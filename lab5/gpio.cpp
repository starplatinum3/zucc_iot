/*
* 演示简单 web Server 功能
* web server 会根据请求做不同的操作
* http://server_ip/gpio/0 打印 /gpio0
* http://server_ip/gpio/1 打印 /gpio1
* Server_IP 就是 ESP8266 的 IP 地址
*/
#include <ESP8266WiFi.h>
//以下三个定义为调试定义
#define DebugBegin(baud_rate) Serial.begin(baud_rate)
#define DebugPrintln(message) Serial.println(message)
#define DebugPrint(message) Serial.print(message)
const char *ssid = "TP-LINK_5344"; //wifi 账号 这里需要修改
const char *password = "xxxx";     //wifi 密码 这里需要修改
// 创建 tcp server
WiFiServer server(80);

void setup()
{
  DebugBegin(115200);
  delay(10);
  // Connect to WiFi network
  DebugPrintln("");
  DebugPrintln(String("Connecting to ") + ssid);
  //STA
  WiFi.mode(WIFI_STA);
  //连接路由 wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    DebugPrint(".");
  }
  DebugPrintln("");
  DebugPrintln("WiFi connected");
  // 启动 server
  server.begin();
  DebugPrintln("Server started");
  // 打印 IP 地址
  DebugPrintln(WiFi.localIP().toString());
}
void loop()
{
  // 等待有效的 tcp 连接
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  DebugPrintln("new client");
  //等待 client 数据过来
  while (!client.available())
  {
    delay(1);
  }
  // 读取请求的第一行 会包括一个 url，这里只处理 url
  String req = client.readStringUntil('\r');
  DebugPrintln(req);
  //清掉缓冲区数据 据说这个方法没什么用 可以换种实现方式

  client.flush();
  // 开始匹配
  int val;
  if (req.indexOf("/gpio/0") != -1)
  {
    DebugPrintln("/gpio0");
    val = 0;
  }
  else if (req.indexOf("/gpio/1") != -1)
  {
    DebugPrintln("/gpio1");
    val = 1;
  }
  else
  {
    DebugPrintln("invalid request");
    //关闭这个 client 请求
    client.stop();
    return;
  }
  //清理缓冲区数据
  client.flush();
  // 准备响应数据
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE 
             HTML >\r\n<html>\r\nGPIO is now ";
             s += (val) ? "high" : "low";
  s += "</html>\n";
  // 发送响应数据给 client
  client.print(s);
  delay(1);
  DebugPrintln("Client disonnected");
  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}