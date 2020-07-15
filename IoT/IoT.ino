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
DHT dht_yl(Temp_and_Hum_PIN_yl, 11);
DHT dht_home(Temp_and_Hum_PIN_home, 11);
Servo servo1;
Adafruit_NeoPixel rgb = Adafruit_NeoPixel(1, RGB, NEO_GRB + NEO_KHZ800);
BLYNK_READ(V1)
{
  float T=dht_home.readTemperature();
   Serial.println("Temperature");
  Serial.println(T);
  Blynk.virtualWrite (V1, T);
}
BLYNK_READ(V3) 
{
  float H=dht_home.readHumidity();
   Serial.println("Hum");
  Serial.println(H);
  Blynk.virtualWrite (V3, H);
}
BLYNK_READ(V4)
{
  float T_1=dht_yl.readTemperature();
   Serial.println("Temperature");
  Serial.println(T_1);
  Blynk.virtualWrite (V4, T_1);
}
BLYNK_READ(V6) 
{
  float H_1=dht_yl.readHumidity();
  Serial.println("Hum");
  Serial.println(H_1);
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
  int value = map(param.asInt(),0,1023,0,180);  
  servo1.write(value);
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
   servo1.write(180);
  } 
  else if (String("Закрыть окно") == param.asStr()) {
   servo1.write(0);
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
  dht_yl.begin();
  dht_home.begin();
  
  
  pinMode(Temp_and_Hum_PIN_home, INPUT);
  pinMode(Temp_and_Hum_PIN_yl, INPUT);
  pinMode(Photores_PIN, INPUT);
  pinMode(PIN,  INPUT);
 // pinMode(dist_PIN, INPUT);
  Blynk.begin(BlynkAuth, WiFiNetwork, WiFiPassword);
}
void loop()
{
  Serial.println(digitalRead(PIN));
  if (digitalRead(PIN)==HIGH)
  {
  Blynk.run(); 
  }
  if (digitalRead(PIN)==LOW)
  {
  Blynk.run();
  //if (
  }
}
