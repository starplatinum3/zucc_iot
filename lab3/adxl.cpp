#include<Wire.h>
#define Addr 0x53
const uint8_t scl=2;
const uint8_t sda=14;

// 连不上 是代码的问题
unsigned int data[6];
int xAccl;
int yAccl;
int zAccl;


void setup() {
  
  Serial.begin(115200);
 
}
// 可以使用
void loop() {
  
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



  xAccl=(((data[1]&0x03)*256)+data[0]);
  if(xAccl>551){
    xAccl-=1024;
  }
    yAccl=(((data[3]&0x03)*256)+data[2]);
  if(yAccl>551){
    yAccl-=1024;
  }

      zAccl=(((data[5]&0x03)*256)+data[4]);
  if(zAccl>551){
    zAccl-=1024;
  }

  Serial.printf("x: %d, y: %d, z: %d\n",xAccl,yAccl,zAccl);

//  display.flipScreenVertically();
//   // display.setFont(ArialMT_Plain_16);
//   display.clear();
  
//    display.drawString(40, 0, "x: "+ String(xAccl));
//      display.drawString(40, 0, "x: "+ String(xAccl));
}
