/**
* Demo：
* 演示 web Server 功能
* 打开 PC 浏览器 输入 IP 地址。请求 web server
*/
#include <ESP8266WiFi.h>

const char *ssid = "xxx";     //wifi 账号 这里需要修改
const char *password = "xxx"; //wifi 密码 这里需要修改

//创建 tcp server 端口号是 80
WiFiServer server(80);
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.printf("Connecting to %s ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  //启动 TCP 连接
  server.begin();
  //打印 TCP server IP 地址
  Serial.printf("Web server started, open %s in a web browser\n",
                WiFi.localIP().toString().c_str());
}
/**
* 模拟 web server 返回 http web 响应内容
* 此处手动拼接 HTTP 响应内容
*/
String prepareHtmlPage()
{
  String htmlPage =
      String("HTTP/1.1 200 OK\r\n") +
      "Content-Type: text/html\r\n" +
      "Connection: close\r\n" + // the connection will be closed after completion of the response
      "Refresh: 5\r\n" +        // refresh the page automatically every 5 sec
      "\r\n" +

      "<!DOCTYPE HTML>" +
      "<html>" +
      "Analog input: " + String(analogRead(A0)) +
      "</html>" +
      "\r\n";
  return htmlPage;
}

void loop()
{
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // 不断读取请求内容
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          //返回响应内容
          client.println(prepareHtmlPage());
          break;
        }
      }
      //由于设置了 Connection: close 当响应数据之后就会自动断开连接
    }
    delay(100); // give the web browser time to receive the data
    // close the connection:
    client.stop();
    Serial.println("[Client disonnected]");
  }
}