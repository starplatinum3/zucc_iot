/**
 * 描述：
* 1.设置固定 IP 192.168.4.1
* 2.设置 webserver 监听 web 请求
* 3.处理 web 请求，获取 ssid 和 pwd
* 4.连接网络
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define DEBUG
#ifdef DEBUG
//以下三个定义为调试定义
#define DebugBegin(baud_rate) Serial.begin(baud_rate)
#define DebugPrintln(message) Serial.println(message)
#define DebugPrint(message) Serial.print(message)
#else
//以下三个定义为调试定义
#define DebugBegin(baud_rate)
#define DebugPrintln(message)
#define DebugPrint(message)
#endif
// const char *ap_ssid = "ESP-CC";       //设置 SSID
// const char *ap_password = "********"; //设置密码

// const char *ap_ssid = "esp-mqp";       //设置 SSID
// const char *ap_password = "xxx"; //设置密码

const char *ap_ssid = "esp-mqp";       //设置 SSID
const char *ap_password = "123456"; //设置密码

char sta_ssid[32] = {0};
char sta_password[64] = {0};
const char *webpage_html = "\
<!DOCTYPE html>\r\n\
<html lang='en'>\r\n\
<head>\r\n\
 <meta charset='UTF-8'>\r\n\
 <title>Document</title>\r\n\
</head>\r\n\
<body>\r\n\
 <form name='input' action='/' method='POST'>\r\n\
 wifi 名称: <br>\r\n\
 <input type='text' name='ssid'><br>\r\n\
 wifi 密码:<br>\r\n\
 <input type='text' name='password'><br>\r\n\
 <input type='submit' value='保存'>\r\n\
 </form>\r\n\
</body>\r\n\
</html>\r\n\
";

// 192.168.43.122
// 有可能是 ip 问题吧，就是 有些ip不能用吗
// http://www.taichi-maker.com/homepage/iot-development/iot-dev-reference/esp8266-c-plus-plus-reference/esp8266wifiap/softapconfig/
IPAddress local_IP(192, 168, 43, 1);
// IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 43, 1);
// IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
void initApConfig();
void initWebServer();
void connectToWifi();
void handleRootPost();
void handleRoot();
void handleNotFound();
ESP8266WebServer server(80);
void setup(void)
{
  // 这里意思是 esp 启动了一个服务器 而不是 手机热点叫做这个
  DebugBegin(115200);
  // 一开始的打印都没有啊 
   DebugPrintln("start");
  DebugPrintln("");
  DebugPrint("connect ap: ");
  DebugPrintln(ap_ssid);

  //  DebugPrintln("connectToWifi");
// connectToWifi();
  initApConfig();

  DebugPrint("IP address: ");
  DebugPrintln(WiFi.softAPIP());
  // 这个时候 他应该已经 ap 模式连接上了，那么服务器怎么没有启动
  // 就算他之后是要链接另外一个网络 但是至少启动服务器这一步他要成功啊
  // 但是没有成功

  // DebugPrintln("设置连接wifi");
  //  WiFi.mode(WIFI_STA);
  // // WiFi.begin(ssid, password);
  //   WiFi.begin(ap_ssid, ap_password);


  initWebServer();
  DebugPrintln("HTTP server started");
  Serial.printf("Ready! Open http://%s in your browser\n",
                WiFi.softAPIP().toString().c_str());
}
void loop(void)
{
  server.handleClient();
}
/**
* 初始化 AP 配置
*/
void initApConfig()
{
  WiFi.mode(WIFI_AP);
  // WIFI_AP 服务器 进不去 arduino
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ap_ssid, ap_password);
}
/**
* 初始化 webserver 配置
*/
void initWebServer()
{
  DebugPrintln("initWebServer");
  server.on("/", HTTP_GET, handleRoot);
  server.on("/", HTTP_POST, handleRootPost);
  server.onNotFound(handleNotFound);
  server.begin();
}
/**
* 连接到 WiFi
*/
void connectToWifi()
{
  // DebugPrintln("connectToWifi");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(sta_ssid, sta_password);
  int cnt = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    cnt++;
    Serial.print(".");
    if (cnt >= 40)
    {
      cnt = 0;
      //重启系统
      DebugPrintln("\r\nRestart now!");
      ESP.restart();
    }
  }
  DebugPrintln("connectToWifi Success!");
}
/**
* 处理 web post 请求
*/
void handleRootPost()
{
  DebugPrintln("handleRootPost");
  if (server.hasArg("ssid"))
  {
    DebugPrint("got ssid:");
    strcpy(sta_ssid, server.arg("ssid").c_str());
    DebugPrintln(sta_ssid);
  }
  else
  {
    DebugPrintln("error, not found ssid");
    server.send(200, "text/html", "<meta charset='UTF-8'>error, not found ssid");
    return;
  }
  if (server.hasArg("password"))
  {
    DebugPrint("got password:");
    strcpy(sta_password, server.arg("password").c_str());
    DebugPrintln(sta_password);
  }
  else
  {
    DebugPrintln("error, not found password");
    server.send(200, "text/html", "<meta charset='UTF-8'>error, not found password");
    return;
  }
  server.send(200, "text/html", "<meta charset='UTF-8'>保存成功");
  delay(2000);
  //连接 wifi
  connectToWifi();
}
/**
* 处理 web get 请求
*/
void handleRoot()
{
  DebugPrintln("handleRoot");
  server.send(200, "text/html", webpage_html);
}
void handleNotFound()
{
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}