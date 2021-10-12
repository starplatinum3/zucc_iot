#include "SSD1306Wire.h"

#include<Wire.h>

SSD1306Wire display(0x3c, 2, 14);


#define Addr 0x53
const uint8_t scl=2;
const uint8_t sda=14;

unsigned int data[6];
int xAccl;
int yAccl;
int zAccl;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //  display.init();
  

}

void loop() {
  // put your main code here, to run repeatedly:
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

  for(int i=0;i<6;i++)
  {
    Wire.beginTransmission(Addr);
    Wire.write((50+i));
    Wire.endTransmission();
    Wire.requestFrom(Addr,1);

    if(Wire.available()==1)
    {
      data[i]=Wire.read();
    }
  }

  xAccl=(((data[1]&0x03)*256)+data[0]);
  if(xAccl>511){
    xAccl-=1024;
  }
   yAccl=(((data[3]&0x03)*256)+data[2]);
  if(yAccl>511){
    yAccl-=1024;
  }
   zAccl=(((data[5]&0x03)*256)+data[4]);
  if(zAccl>511){
    zAccl-=1024;
  }

    Serial.printf("x: %d, y: %d, z: %d\n",xAccl,yAccl,zAccl);

    display.init();
    // 需要每次都init
 display.flipScreenVertically();
  // display.setFont(ArialMT_Plain_16);
  display.clear();
  
  // 第一个参数是 横着的 x 
   display.drawString(0, 0, "x: "+ String(xAccl));
  //    display.drawString(20, 0, "y: "+ String(yAccl));
  //  display.drawString(0, 20, "z: "+ String(zAccl));
     display.drawString(0, 20, "y: "+ String(yAccl));
   display.drawString(0, 40, "z: "+ String(zAccl));
     display.display();
      delay(500);
}
