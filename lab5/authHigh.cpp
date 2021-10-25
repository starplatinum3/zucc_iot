#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <dht11.h>

dht11 DHT11;

#ifndef STASSID
// #define STASSID "GreenIoT"
// #define STAPSK "******"

#define STASSID "xxx"
#define STAPSK "xxx"

#endif
const char *ssid = STASSID;
const char *password = STAPSK;
ESP8266WebServer server(80);
//Check if header is present and correct
bool is_authenticated()
{
  Serial.println("Enter is_authenticated");
  if (server.hasHeader("Cookie"))
  {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1)
    {
      Serial.println("Authentication Successful");
      return true;
    }
  }
  Serial.println("Authentication Failed");
  return false;
}
//login page, also called for disconnect
void handleLogin()
{
  Serial.println("handleLogin");
  String msg;
  if (server.hasHeader("Cookie"))
  {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT"))
  {
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }

  Serial.println("server.hasArg(USERNAME)");
  Serial.println(server.hasArg("USERNAME"));

  Serial.println("server.hasArg(PASSWORD)");
  Serial.println(server.hasArg("PASSWORD"));
  // 所以没有这个 参数吗
  // server.hasArg("USERNAME")  没有获取到
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD"))
  {
    String USERNAME = server.arg("USERNAME");
    Serial.println("USERNAME");
    Serial.println(USERNAME);
    String PASSWORD = server.arg("PASSWORD");
    Serial.println("PASSWORD");
    Serial.println(PASSWORD);
    if (server.arg("USERNAME") == "admin" && server.arg("PASSWORD") == "admin")
    {
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }
    msg = "Wrong username/password! try again.";
    Serial.println("Log in Failed");
  }
  Serial.println("send to ");
  String content = "<html><body><form action='/login' method='POST'>To log in, please use : admin /admin<br> ";
  content += "User:<input type='text' name='USERNAME' placeholder='username'><br>";
  content += "Password:<input type='password' name='PASSWORD'  placeholder = 'password' ><br> ";
  // 这里type 复制的时候 有空格 =' password '，怪不得有问题了
  content += "<input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "<br>";
  content += "You also can go <a href='/inline'>here</a></body></html>";
  server.send(200, "text/html", content);
}
//root page can be accessed only if authentication is ok
void handleRoot()
{
  Serial.println("Enter handleRoot");
  String header;
  if (!is_authenticated())
  {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  String content = "<html><body><H2>hello, you successfully connected to esp8266 !</H2><br> ";
  if (server.hasHeader("User-Agent"))
  {
    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
  }
  Serial.println("send to ");
  content += "You can a href =\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
  server.send(200, "text/html", content);
}

//no need authentication
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void gpio()
{
  Serial.println("Enter gpio");
  String header;
  if (!is_authenticated())

  {
    // sendHeader 这是之后的重定向吧
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  int dhtIo = 5;
  DHT11.read(dhtIo);
  // String content = "<html><body><H2>g0 :"+String(digitalRead(0))+" </H2> <br> <H2>g0 :"+String(digitalRead(1))+" </H2> <br> ";
  // invalid operands of types 'const char [19]' and 'const char [11]' to binary 'operator+'
  //
  String content = String("") + "<html><body> <div>" +
                   " <h2>g0 : " + String(digitalRead(0)) + " </h2> <br>" +
                   " <h2>g1 :" + String(digitalRead(1)) + " </h2><br>" +
                   "  <h2>31901077 </h2> <br> " +
                   " <h2>mqp </h2><br>" +
                   " <h2>temperature " + String(DHT11.temperature) + "  </h2><br>" +
                   " <h2>humidity  " +
                   String(DHT11.humidity) + "</h2><br>" +
                   " </div> ";
  if (server.hasHeader("User-Agent"))
  {
    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
  }
  content += "You can access this page until you <a href =\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
  server.send(200, "text/html", content);
}

// int dhtIo = 5;
// 这里定义的 gpio 函数收不到
// 只有 setup 可以收到吗
// void readDht(){
//     DHT11.read(dhtIo);
//     String(DHT11.temperature);
// }
void setup(void)
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works without need of authentication");
  });
  server.on("/gpio", gpio);
  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char *headerkeys[] = {"User-Agent", "Cookie"};
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);

  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  Serial.println("HTTP server started");
}
void loop(void)
{
  server.handleClient();
}