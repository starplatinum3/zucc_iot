#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"
#include "font.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

SSD1306Wire display(0x3c, 0, 2);

const char* ssid = "wifi";
const char* password = "123456";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  char string[length];
  memcpy(string, payload,length);
  string[length] = '\0';
  display.drawString(0,0,string);
  display.display();
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-label";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      client.publish("/iot/label", "ready");
      client.subscribe("/iot/label");
    } else {
    }
  }
}
void setup() {
  display.init();
  display.flipScreenVertically();
  display.setFont(Font30);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void drawTextAlignmentDemo() {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");
}

//https://zhuanlan.zhihu.com/p/111482208

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(10);
}