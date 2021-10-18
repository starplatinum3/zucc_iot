/*
 This sketch establishes a TCP connection to a "quote of the day" service.
 It sends a "hello" message, and then prints received data.
*/
#include <ESP8266WiFi.h>
#ifndef STASSID
// #define STASSID "NETGEAR" 
// #define STAPSK "*******" 

#define STASSID "gerogerori" 
#define STAPSK "****" 

#endif
const char* ssid = STASSID;
const char* password = STAPSK;
// const char* host = "192.168.1.220";
// 192.168.43.10
// const char* host = "169.254.97.243";
// const char* host = "192.168.43.10";
// 这个host 不是和 wifi ip 一样的
// const char* host = "192.168.43.220";
const char* host = "192.168.43.122"; 



const uint16_t port = 6800;
 // Use WiFiClient class to create TCP connections
WiFiClient client;


#include<Wire.h>
#define Addr 0x53
const uint8_t scl=2;
const uint8_t sda=14;

// 连不上 是代码的问题
// unsigned int data[6];
// int xAccl;
// int yAccl;
// int zAccl;

#include<vector>


 std::vector<int> readAccl(){
  
  unsigned int data[6];

  Wire.beginTransmission(Addr);
  Wire.begin(sda,scl);
  Wire.write(0x2C);
  Wire.write(0x0A);
  Wire.endTransmission();
  

  Wire.beginTransmission(Addr);
  Wire.write(0x2D);
  Wire.write(0x08);
  Wire.endTransmission();

Wire.beginTransmission(Addr);

  Wire.write(0x31);
  Wire.write(0x08);
  Wire.endTransmission();
   delay(300);
// 少了 delay 就有问题 好像
  for(int i=0;i<6;i++){
     Wire.beginTransmission(Addr);
     Wire.write((50+i));
     Wire.endTransmission();
       Wire.requestFrom(Addr,1);
  if(Wire.available()==1){
    data[i]=Wire.read();
  }
  }

  int  xAccl=(((data[1]&0x03)*256)+data[0]);
  if(xAccl>551){
    xAccl-=1024;
  }

int  yAccl=(((data[3]&0x03)*256)+data[2]);
  if(yAccl>551){
    yAccl-=1024;
  }

  int   zAccl=(((data[5]&0x03)*256)+data[4]);
  if(zAccl>551){
    zAccl-=1024;
  }

 std:: vector<int>v;
 
  v.push_back(xAccl);
  v.push_back(yAccl);
  v.push_back(zAccl);
  return v;

}

// int getXAccl(){
//     xAccl=(((data[1]&0x03)*256)+data[0]);
//   if(xAccl>551){
//     xAccl-=1024;
//   }
//   return xAccl;
// }


// int getYAccl(){
//    yAccl=(((data[3]&0x03)*256)+data[2]);
//   if(yAccl>551){
//     yAccl-=1024;
//   }
//    return yAccl;
// }


// int getZAccl(){
 
//       zAccl=(((data[5]&0x03)*256)+data[4]);
//   if(zAccl>551){
//     zAccl-=1024;
//   }
//    return zAccl;
// }




void setup() {
 Serial.begin(115200);
 // We start by connecting to a WiFi network
 Serial.println();
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);
 /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
 would try to act as both a client and an access-point and could cause
 network-issues with your other WiFi-devices on your WiFi-network. */
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
//  host=WiFi.localIP();
//  conversion from 'IPAddress' to 'const char*' is ambiguous
// 
}
void loop() {
 Serial.print("connecting to ");
 Serial.print(host);
 Serial.print(':');
 Serial.println(port);
// 物联网技术与应用
// 10
 if (!client.connect(host, port)) {
 Serial.println("connection failed");
 delay(5000);
 return;
 }
 // This will send a string to the server
 Serial.println("sending data to server");
  std:: vector<int> accls= readAccl();
 int xAccl=accls[0];
 int yAccl=accls[1];
 int zAccl=accls[2];
//  xAccl=getXAccl();
 if (client.connected()) {
//  client.println("hello from ESP8266 from mqp");
  // client.println( "Hello from ESP8266 , Hello  from mqp ");
   client.printf("x: %d, y: %d, z: %d\n",xAccl,yAccl,zAccl);

 }
 // wait for data to be available
// unsigned long timeout = millis();
// while (client.available() == 0) {
// if (millis() - timeout > 5000) {
// Serial.println(">>> Client Timeout !");
// client.stop();
 delay(6000);
// return;
// }
// }
 // Read all the lines of the reply from server and print them to Serial
 Serial.println("receiving from remote server");
 // not testing 'client.connected()' since we do not need to send data here
 while (client.available()) {
 char ch = static_cast<char>(client.read());
 Serial.print(ch);
 }
 // Close the connection
 Serial.println();
 Serial.println("closing connection");
 client.stop();
 delay(3000); // execute once every 5 minutes, don't flood remote service
}
