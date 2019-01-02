#include <Arduino.h>
#include <wire.h>
#include "SSD1306.h"
#include <NeoPixelBus.h>

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFipwd.h>

SSD1306  display(0x3c, 21, 22);

const uint16_t PixelCount = 7;
const uint8_t PixelPin = 2;
#define colorSaturation 255

NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

RgbwColor red(colorSaturation, 0, 0, 0);
RgbwColor green(0, colorSaturation, 0, 0);
RgbwColor blue(0, 0, colorSaturation, 0);
RgbwColor white1(colorSaturation, colorSaturation, colorSaturation, 0);
RgbwColor white2(0,0,0,colorSaturation);
RgbwColor white3(colorSaturation, colorSaturation, colorSaturation, colorSaturation);
RgbwColor mult(0,colorSaturation,0,0);
RgbwColor black(0);
uint8_t gg=0;
uint8_t ff=0;

////
// in WiFipwd.h
//const char *ssid     = "ssid";
//const char *password = "password";
////
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
uint8_t oldt = 255;

const uint8_t Touch1 = 36;

void setup() {
 
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.drawString(0, 0, "Hello World");
  display.display();

  strip.Begin();
  strip.Show();

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    display.clear();
    delay ( 200 );
    display.drawString(0, 0, "Connect Wifi");
    display.display();
    delay ( 300 );
  }

    display.clear();
    display.drawString(0, 0, "Wifi Connected");
    display.invertDisplay();
    display.display();


  timeClient.begin();

  pinMode(Touch1, INPUT);

}
 
void loop() {

  if(digitalRead(Touch1) == 1) {
    strip.SetPixelColor(6, mult);
    strip.Show();
  }
  else {
    strip.SetPixelColor(6, black);
    strip.Show();
  }
  

  if(timeClient.getSeconds() != oldt) {
    timeClient.update();

    display.clear();
    display.drawString(0, 0, timeClient.getFormattedTime());
    display.display();
    oldt = timeClient.getSeconds();

    if(ff == 0) {
      strip.SetPixelColor(0, red);
      strip.SetPixelColor(1, green);
      strip.SetPixelColor(2, blue);
      strip.SetPixelColor(3, white1);
      strip.SetPixelColor(4, white2);
      strip.SetPixelColor(5, white3);
      strip.Show();
      ff=1;
    }
    else {
      strip.SetPixelColor(0, black);
      strip.SetPixelColor(1, black);
      strip.SetPixelColor(2, black);
      strip.SetPixelColor(3, black);
      strip.SetPixelColor(4, black);
      strip.SetPixelColor(5, black);
      strip.Show();
      ff=0;
    }
  }
}