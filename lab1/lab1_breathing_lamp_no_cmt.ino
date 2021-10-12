

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{

  int val = 1024;

  int rangeFrom = 500;
  int rangeTo = 1023;
 
  int ledRangeFrom = 500;

  int ledPwm = 1;

  int ledPwm2 = 2047;

  int led1Top = 2048;
  int led2Top = 2048;

  int cnt = 500;

  for (int i = 0; i < cnt && ledPwm > 0 && ledPwm < led1Top && ledPwm2 > 0 && ledPwm2 < led2Top; i++)
  {
    analogWrite(LED_BUILTIN, ledPwm);

    ledPwm += (led1Top * 1.0 / cnt);
    
    analogWrite(4, ledPwm2);
    
    ledPwm2 -= (led2Top * 1.0 / cnt);
    
    delay(10);
   
  }

  
  ledPwm = 2047;
  ledPwm2 = 1;
  
  for (int i = 0; i < cnt && ledPwm > 0 && ledPwm < led1Top && ledPwm2 > 0 && ledPwm2 < led2Top; i++)
  {

    analogWrite(LED_BUILTIN, ledPwm);
    

    ledPwm -= (led1Top * 1.0 / cnt);

    analogWrite(4, ledPwm2);
  
    ledPwm2 += (led2Top * 1.0 / cnt);
 
    delay(10);

    
  }

  
}
