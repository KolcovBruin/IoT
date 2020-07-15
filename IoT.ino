#define BLYNK_PRINT Serial 
#include <ESP8266WiFi.h>      
#include <BlynkSimpleEsp8266.h>  
#include <Adafruit_NeoPixel.h>   
#include "DHT.h"
#include <Servo.h>
char BlynkAuth[] = "K69NTayfvJ7vPBuIvU1WyNFY4rSMxG--";
char WiFiNetwork[] = "Link";
char WiFiPassword[] = "9110224141";
#define RGB                   4 
#define Temp_and_Hum_PIN_yl   12
#define Photores_PIN          16
#define Temp_and_Hum_PIN_home 13
#define PIN                   5
#define Servo_PIN             15
#define Low_light             55
#define High_light            255
float Temp_Home = 0;
float Temp_Str = 0;
uint32_t rgbColor;
int angle = 0;
DHT dht_st(Temp_and_Hum_PIN_yl, 11);
DHT dht_home(Temp_and_Hum_PIN_home, 11);
Servo servo1;
Adafruit_NeoPixel rgb = Adafruit_NeoPixel(1, RGB, NEO_GRB + NEO_KHZ800);
BLYNK_READ(V1)
{
  Temp_Home=dht_home.readTemperature();
  Blynk.virtualWrite (V1, Temp_Home);
}
BLYNK_READ(V3) 
{
  float H=dht_home.readHumidity();

  Blynk.virtualWrite (V3, H);
}
BLYNK_READ(V4)
{
  Temp_Str=dht_st.readTemperature();

  Blynk.virtualWrite (V4, Temp_Str);
}
BLYNK_READ(V6) 
{
  float H_1=dht_st.readHumidity();
  Blynk.virtualWrite (V6, H_1);
}
BLYNK_READ(V5)
{
  if (digitalRead(Photores_PIN)==1)
  {
  Blynk.virtualWrite (V5, "Темно");
  }
  if (digitalRead(Photores_PIN)==0)
  {
  Blynk.virtualWrite (V5, "Светло");
  }
}

BLYNK_WRITE(V7){  
   angle = map(param.asInt(),0,1023,0,180);  
  servo1.write(angle);
}
WidgetTerminal terminal(V2);
BLYNK_WRITE(V2)
{
  if (String("Green") == param.asStr()) {
   uint32_t rgbColor=rgb.Color(0,255,0);
    rgb.setPixelColor(0, rgbColor);
    rgb.show();
  } 
  else if (String("Red") == param.asStr()) {
   uint32_t rgbColor=rgb.Color(255,0,0);
    rgb.setPixelColor(0, rgbColor);
    rgb.show();
  } 
 else if (String("Blue") == param.asStr()) {
   uint32_t rgbColor=rgb.Color(0,0,255);
    rgb.setPixelColor(0, rgbColor);
    rgb.show();
  } 
 else if (String("off") == param.asStr()) {
   uint32_t rgbColor=rgb.Color(0,0,0);
    rgb.setPixelColor(0, rgbColor);
    rgb.show();
  } 
  else if (String("Открыть окно") == param.asStr()) {
    angle=180;
   servo1.write(angle);
  } 
  else if (String("Закрыть окно") == param.asStr()) {
    angle=0;
   servo1.write(angle);
  }
  
  terminal.flush();
  delay(500);
  terminal.clear();
}
void setup()
{
  servo1.attach(Servo_PIN, 900, 2100);
  Serial.begin(9600); 
  rgb.begin(); 
  dht_st.begin();
  dht_home.begin();
  
  servo1.write(0);
  pinMode(Temp_and_Hum_PIN_home, INPUT);
  pinMode(Temp_and_Hum_PIN_yl, INPUT);
  pinMode(Photores_PIN, INPUT);
  pinMode(PIN,  INPUT);
 // pinMode(dist_PIN, INPUT);
  Blynk.begin(BlynkAuth, WiFiNetwork, WiFiPassword);
}
void loop()
{
  
  //Serial.println(digitalRead(PIN));
  if (digitalRead(PIN)==HIGH)
  {
  Blynk.run(); 
  }
  if (digitalRead(PIN)==LOW)
  {
    int Lvl_light=0;
    if (digitalRead(Photores_PIN)==LOW)
  {
    Lvl_light=High_light;
  }
  else
  {
     Lvl_light=Low_light;
  }
  Blynk.run();
  //охлаждение если жарко

  if (Temp_Home>25) 
  {
    //охлаждение с окна
    if (Temp_Str<=25 && angle==0)  //если на улице холднее и окно закрыто, то выключить кондей и открыть окно
    {
      if (rgbColor!=rgb.Color(Low_light,Low_light,Low_light)||rgbColor!=rgb.Color(High_light,High_light,High_light))  //если кондей вкл, то выключить его
      {
       rgbColor=rgb.Color(Lvl_light,Lvl_light,Lvl_light);
      rgb.setPixelColor(0, rgbColor);
      rgb.show();
      }
      angle=90;
      servo1.write(angle);
    }
    //кондиционер 
    else if (Temp_Str>25&&(rgbColor!=rgb.Color(0,0,Low_light)||rgbColor!=rgb.Color(0,0,High_light))) //если на улице жарче и кондей выключен, то зыкрыть окно и вкл кондей
    { 
      if (angle!=0)  //если окно открыто, то закрыть
      {
        angle=0;
        servo1.write(angle);
      }
      rgbColor=rgb.Color(0,0,Lvl_light);
      rgb.setPixelColor(0, rgbColor);
      rgb.show();
    }
  }
  else if (Temp_Home<24.9)  //если дома нормально, то закрыть окно или выключить кондей
  {
    if (angle!=0)  //если окно открыто, то закрыть
    {
      angle=0;
      servo1.write(angle);
    }
    if ((rgbColor==rgb.Color(0,0,Low_light)||rgbColor==rgb.Color(High_light,0,0)))  //если кондей вкл, то выкл
    {
      rgbColor=rgb.Color(Lvl_light,Lvl_light,Lvl_light);
      rgb.setPixelColor(0, rgbColor);
      rgb.show();
    }
  }
  if (Temp_Home<24.3) //если дома холоднее нормы
  {
    if (Temp_Str>24.3&&angle==0) //на улице теплее, то открыть окно
    {
     if (rgbColor==rgb.Color(Low_light,0,0)||rgbColor==rgb.Color(High_light,0,0))
     {
     
      rgbColor=rgb.Color(Lvl_light,Lvl_light,Lvl_light);
      rgb.setPixelColor(0, rgbColor);
      rgb.show();
     }
      angle=90;
      servo1.write(angle);
    }
    if (Temp_Str<24.3&&(rgbColor!=rgb.Color(Low_light,0,0)||rgbColor!=rgb.Color(High_light,0,0))) //на улице холоднее, закрыть окно и вкл обогрев
    {
      if(angle!=0) //открыто окно, закрыть
      {
        angle=0;
        servo1.write(angle);
      }
      
      rgbColor=rgb.Color(Lvl_light,0,0);
      rgb.setPixelColor(0, rgbColor);
      rgb.show();
    }
  }
  else if(Temp_Home>24.5&&Temp_Home<25) //дома норма, закрыть окно и выключить обогрев
  {
    if (angle!=0)
    {
      angle=0;
      servo1.write(angle);
    }
    if (rgbColor==rgb.Color(Low_light,0,0)||rgbColor==rgb.Color(High_light,0,0))
    {
      rgbColor=rgb.Color(Lvl_light,Lvl_light,Lvl_light);
      rgb.setPixelColor(0, rgbColor);
      rgb.show();
    }
  }
// сценарий про свет
  
  }
}
