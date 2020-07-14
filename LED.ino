#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>      
#include <BlynkSimpleEsp8266.h>  // http://librarymanager/All#blynk 
#include <Adafruit_NeoPixel.h>   // http://librarymanager/All#neopixel
#include "DHT.h"
#include <Servo.h>
////////////////////
// Blynk Settings //
////////////////////
char BlynkAuth[] = "K69NTayfvJ7vPBuIvU1WyNFY4rSMxG--";
char WiFiNetwork[] = "Link";
char WiFiPassword[] = "9110224141";

///////////////////////
// Hardware Settings //
///////////////////////
#define WS2812_PIN 4 // Pin connected to WS2812 LED
#define BUTTON_PIN 0
#define LED_PIN    5
DHT dht(13, 11);
Servo servo1;
Servo servo2;
Adafruit_NeoPixel rgb = Adafruit_NeoPixel(1, WS2812_PIN, NEO_GRB + NEO_KHZ800);

BLYNK_WRITE(V0) // Handle RGB from the zeRGBa
{
  if (param.getLength() < 5)
    return;

  byte red = param[0].asInt();
  Serial.println(red);
  delay(1000);
  byte green = param[1].asInt();
  Serial.println(green);
  delay(1000);
  byte blue = param[2].asInt();
  Serial.println(blue);
  delay(1000);

  uint32_t rgbColor = rgb.Color(red, green, blue);
  rgb.setPixelColor(0, rgbColor);
  rgb.show();
  
}

BLYNK_READ(V1) // Handle RGB from the zeRGBa
{
  float T=dht.readTemperature();//Температура
  Blynk.virtualWrite (V1, T);
}
BLYNK_READ(V3) // Handle RGB from the zeRGBa
{
  float H=dht.readHumidity();//Влажность
  Blynk.virtualWrite (V3, H);
}
BLYNK_READ(V4) // Handle RGB from the zeRGBa
{
  Blynk.virtualWrite (V4, digitalRead(16));
}
BLYNK_READ(V5) // Handle RGB from the zeRGBa
{
  Blynk.virtualWrite (V5, "aaa");
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

  // Ensure everything is sent
  terminal.flush();
  delay(500);
  terminal.clear();
}
void setup()
{
  servo1.attach(15, 900, 2100);
  // Initialize hardware
  Serial.begin(9600); // Serial
  rgb.begin(); // RGB LED
  dht.begin();
  
  pinMode(BUTTON_PIN, INPUT); // Button input
  pinMode(LED_PIN, OUTPUT); // LED output
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(5, INPUT);
  pinMode(5, OUTPUT);
  pinMode(16, INPUT);
 // pinMode(15, INPUT);
  
  // Initialize Blynk
  Blynk.begin(BlynkAuth, WiFiNetwork, WiFiPassword);
  
}

void loop()
{
  
  Serial.println(digitalRead(5));
  if (digitalRead(5)==HIGH)
  {
  analogWrite(5,122);
  Blynk.run(); 
  //servo1.write(10);
  }
  if  (digitalRead(5)==LOW)
  {
  Blynk.run();
  Serial.println(000);
  Serial.print("objekt:  ");
  Serial.println(analogRead(16));
  Serial.print("light:  ");
  Serial.println(analogRead(12));
  analogWrite(5,122);
  servo1.write(90);    
  delay(1000);         
  servo1.write(180);   
  delay(1000);         
  servo1.write(0);     
  delay(1000);

  }
 // delay(500);
}
