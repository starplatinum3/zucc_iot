#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID "xxx"
#define WLAN_PASS "xxx"

#define AIO_SERVER "139.196.xx.x"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "xx"
#define AIO_KEY "xxx"

#include "SSD1306Wire.h"
SSD1306Wire display(0x3c, 2, 14);
#include <dht11.h>
#include <ArduinoJson.h>

dht11 DHT11;
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

StaticJsonDocument<200> doc;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish pubTemp = Adafruit_MQTT_Publish(&mqtt, "ZUCC-MQP/esp");

// Setup a feed called 'onoff' for subscribing to changes.
// Adafruit_MQTT_Subscribe subRgb = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "iot/2");
Adafruit_MQTT_Subscribe subRgb= Adafruit_MQTT_Subscribe(&mqtt,"ZUCC-MQP/rgb");
Adafruit_MQTT_Subscribe subOled = Adafruit_MQTT_Subscribe(&mqtt, "ZUCC-MQP/oled");
// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

// MQTT topic 命名规则（ZUCC-IOT姓名缩写，比如”ZUCC-IOTCJP”）：
// ZUCC-CJP/esp：小程序用于接收来自 ESP8266 板的温湿度数据，在小程序端可以
// 加载 echarts 图表显示温湿度值；
// ZUCC-CJP/rgb：小程序控制 ESP8266 板上的 RGB 灯的显示，小程序端可以用调
// 色盘设定 RGB 值，用于控制 ESP8266 板上的 RGB 灯显示；
// ZUCC-CJP/oled：用于输出字符，显示在 ESP8266 的 OLED 屏上。输出字符必须
// 包含本人姓名缩写和学号，同时 OLED 屏要显示实时温湿度数值，中文字符显示可选。

const int pin_r = 12;
const int pin_g = 13;
const int pin_b = 15;
// 这个应该和gpio一样的意思吧

void rgbSetup()
{
  pinMode(pin_r, OUTPUT);
  pinMode(pin_g, OUTPUT);
  pinMode(pin_b, OUTPUT);
}

// 关闭 rgb arduino
void drawRgb(int r, int g, int b)
{
  analogWrite(pin_r, r);
  analogWrite(pin_g, g);
  analogWrite(pin_b, b);
}

void setup()
{

  rgbSetup();
  Serial.begin(115200);
  delay(10);
  display.init();

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&subRgb);
}

int strSame(char *str, char *str2)
{
  return strcmp(str, str2) == 0;
}
#include <string.h>
uint32_t x = 0;

void displayRead(Adafruit_MQTT_Subscribe sub){
  Serial.print(F("Got: "));
      char *read = (char *)sub.lastread;

      // Serial.println((char *)subOled.lastread);
 Serial.println(read);
      display.flipScreenVertically();
      display.clear();
      display.drawString(0, 10, "topic: iot/1");
      display.drawString(0, 20,read);
      display.display();
}
void loop()
{
  int chk = DHT11.read(5);
  // Ensure the connection to the MQTT server is alive (this will make the first
  //  connection  and automatically reconnect when  disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if (subscription == &subRgb)
    {
      Serial.print(F("Got: "));
      Serial.println((char *)subRgb.lastread);

      char *read = (char *)subRgb.lastread;

      display.flipScreenVertically();
      display.clear();
      display.drawString(0, 10, "topic: iot/1");
      display.drawString(0, 20, (char *)subRgb.lastread);
      display.display();

      // drawRgb(111, 111, 111);

            StaticJsonDocument<200> ledJson;
      //   deserializeJson(ledJson,(char*)subRgb.lastread);
          deserializeJson(ledJson,read);
        drawRgb(ledJson["r"],ledJson["g"],ledJson["b"]);

      //        if(strcmp(read,"1")==0){
      // //   drawRgb()

      // StaticJsonDocument<200> ledJson;
      //   deserializeJson(ledJson,(char*)read.lastread);
      //   drawRgb(ledJson["r"],ledJson["g"],ledJson["b"]);
      //     //   analogWrite(12, led["valueR"]);
      //     //   analogWrite(13, led["valueG"]);
      //     //   analogWrite(15, led["valueB"]);
      //  }

      delay(2000);
    }else if(subscription == &subOled){
      displayRead(subOled);

    }
  }
  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");

  String output;

  doc["hum"] = (float)DHT11.humidity;
  doc["temp"] = (float)DHT11.temperature;
  serializeJson(doc, output);

  if (!photocell.publish(output.c_str()))
  {
    Serial.println(F("Failed"));
  }
  else
  {
    Serial.println(F("OK!"));

    display.flipScreenVertically();
    display.clear();
    display.drawString(0, 10, "topic: iot/1");
    display.drawString(0, 20, String(x));
    //  display.display();
    delay(2000);
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
if(! mqtt.ping()) { mqtt.disconnect();
}
*/
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect()
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected())
  {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)
  { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); // wait 5 seconds retries--;
    if (retries == 0)
    {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("MQTT Connected!");
}

// json  6