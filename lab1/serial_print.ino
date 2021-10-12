void setup()
{
  // Serial.begin(9600);
   Serial.begin(115200);
}

void loop()
{
  FlashMode_t ideMode = ESP.getFlashChipMode();
  String coreVersion = ESP.getCoreVersion();
  Serial.print(F("Arduino Core For ESP8266 Version"));
  Serial.println(coreVersion);
  Serial.printf("Flash real id: %08X\n", ESP.getFlashChipId());
  delay(1000);
}