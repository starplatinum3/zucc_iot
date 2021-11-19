#include <Wire.h>
#define Addr 0x53
const uint8_t scl = 2;
const uint8_t sda = 14;
const int pin_r = 12;
const int pin_g = 13;
const int pin_b = 15;
// 这个应该和gpio一样的意思吧

unsigned int data[6];
int xAccl;
int yAccl;
int zAccl;
// 陀螺仪的数据转化为 rgb

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pin_r, OUTPUT);
  pinMode(pin_g, OUTPUT);
  pinMode(pin_b, OUTPUT);
}

void drawRgb(int r, int g, int b)
{
  analogWrite(pin_r, r);
  analogWrite(pin_g, g);
  analogWrite(pin_b, b);
}
void loop()
{
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(Addr);
  Wire.begin(sda, scl);
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

  for (int i = 0; i < 6; i++)
  {
    Wire.beginTransmission(Addr);
    Wire.write((50 + i));
    Wire.endTransmission();
    Wire.requestFrom(Addr, 1);

    if (Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
  }

  Serial.printf("data \n");
  for (int i = 0; i < 6; i++)
  {
    Serial.printf("%d ", data[i]);
  }
  Serial.printf("\n");

  xAccl = (((data[1] & 0x03) * 256) + data[0]);
  // 加速度
  if (xAccl > 511)
  {
    xAccl -= 1024;
  }
  yAccl = (((data[3] & 0x03) * 256) + data[2]);
  if (yAccl > 511)
  {
    yAccl -= 1024;
  }
  zAccl = (((data[5] & 0x03) * 256) + data[4]);
  if (zAccl > 511)
  {
    zAccl -= 1024;
  }

  // analogWrite(12, 255);
  drawRgb(data[0], data[2], data[4]);

  Serial.printf("x: %d, y: %d, z: %d\n", xAccl, yAccl, zAccl);

  //  delay(1000);
}
