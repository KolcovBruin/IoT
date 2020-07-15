#define BLYNK_PRINT Serial 
#include <ESP8266WiFi.h>      
#include <BlynkSimpleEsp8266.h>  
#include <Adafruit_NeoPixel.h>   
#include "DHT.h"
#include <Servo.h>
char BlynkAuth[] = "K69NTayfvJ7vPBuIvU1WyNFY4rSMxG--";
char WiFiNetwork[] = "Link";
char WiFiPassword[] = "9110224141";
#define RGB              4 
#define dist_PIN         16
#define Photores_PIN     12
#define Temp_and_Hum_PIN 13
#define PIN              5
#define Servo_PIN        15
DHT dht(Temp_and_Hum_PIN, 11);
Servo servo1;
Adafruit_NeoPixel rgb = Adafruit_NeoPixel(1, RGB, NEO_GRB + NEO_KHZ800);
BLYNK_READ(V1)
{
  float T=dht.readTemperature();
  Blynk.virtualWrite (V1, T);
}
BLYNK_READ(V3) 
{
  float H=dht.readHumidity();
  Blynk.virtualWrite (V3, H);
}
BLYNK_READ(V4) 
{
  if (digitalRead(dist_PIN)==1)
  {
  Blynk.virtualWrite (V4, "Нет объекта");
  }
  if (digitalRead(dist_PIN)==0)
  {
  Blynk.virtualWrite (V4, "Есть объект");
  }
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
  
  terminal.flush();
  delay(500);
  terminal.clear();
}
void setup()
{
  servo1.attach(Servo_PIN, 900, 2100);
  Serial.begin(9600); 
  rgb.begin(); 
  dht.begin();
  
  pinMode(Temp_and_Hum_PIN, INPUT);
  pinMode(Photores_PIN, INPUT);
  pinMode(PIN,  INPUT);
  pinMode(dist_PIN, INPUT);
  Blynk.begin(BlynkAuth, WiFiNetwork, WiFiPassword);
}
void loop()
{
  Serial.println(digitalRead(5));
  if (digitalRead(5)==HIGH)
  {
  Blynk.run(); 
  }
  if  (digitalRead(5)==LOW)
  {
  Blynk.run();
  Serial.print("objekt:  ");
  Serial.println(analogRead(16));
  Serial.print("light:  ");
  Serial.println(analogRead(12));
  servo1.write(90);    
  delay(1000);         
  servo1.write(180);   
  delay(1000);         
  servo1.write(0);     
  delay(1000);
  }
}
