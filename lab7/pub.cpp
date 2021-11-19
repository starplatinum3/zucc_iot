#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Update these with values suitable for your network.
// const char *ssid = "********";               //wifi 账号
// const char *password = "********";           //wifi 秘密
// const char *mqtt_server = "***.***.***.***"; //mqtt 服务器

const char *ssid = "xxx";               //wifi 账号
const char *password = "xxx";           //wifi 秘密
const char *mqtt_server = "broker.emqx.io"; //mqtt 服务器
const char *outTopic = "outMqp"; //mqtt 服务器
const char *inTopic = "inMqp"; //mqtt 服务器
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
/**
* 消息回调
*/
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  //如果接收到1作为第一个字符，则打开LED
  if ((char)payload[0] == '1')
  {
    digitalWrite(BUILTIN_LED, LOW); // Turn the LED on (Note that LOW is the voltage   level
    //打开LED（注意电压水平低
  
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
    //但实际上LED是亮着的；这是因为
//它在ESP-01上处于低激活状态）
  }
  else
  {
    digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off by making the voltage HIGH
    //通过使电压高来关闭LED
  }
}
/**
* 断开重连
*/
void reconnect()
{
  // Loop until we're reconnected
  //循环直到我们重新连接
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "hello world");
      // ... and resubscribe
      client.subscribe(inTopic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup()
{
  pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  //配置 mqtt 服务器地址和端口
  client.setServer(mqtt_server, 1883);
  //设置订阅消息回调
  client.setCallback(callback);
}
void loop()
{
  //重连机制
  if (!client.connected())
  {
    reconnect();
  }
  //不断监听信息
  client.loop();
  long now = millis();
  if (now - lastMsg > 2000)
  {
    //每 2s 发布一次信息
    lastMsg = now;
    ++value;
    snprintf(msg, 50, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}