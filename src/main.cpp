#include <Arduino.h>
#include <wire.h>
#include "SSD1306.h"
#include <NeoPixelBus.h>

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFipwd.h>

// The display
SSD1306  display(0x3c, 21, 22);

////
// in WiFipwd.h
//const char *ssid     = "ssid";
//const char *password = "password";
////
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "192.168.42.99", 3600, 60000); //todo: adjust timeoffset to DST automatically

// Char LEDs (Matrix)
const uint8_t PanelWidth = 11;
const uint8_t PanelHeight = 10;
const uint16_t PanelPixelCount = PanelWidth * PanelHeight;
const uint8_t PanelPixelPin = 2;
NeoTopology <RowMajorLayout> topo(PanelWidth, PanelHeight);
NeoPixelBus <NeoGrbwFeature, Neo800KbpsMethod> PanelStrip(PanelPixelCount, PanelPixelPin);

// Other LEDs (minutes 1-4, 8 touch keys)
const uint16_t OtherPixelCount = 12;
const uint8_t OtherPixelPin = 3;
NeoPixelBus <NeoGrbwFeature, Neo800KbpsMethod> OtherStrip(PanelPixelCount, PanelPixelPin);

// globals
uint8_t minBrightness = 10;
uint8_t maxBrightness = 255;
uint8_t oldt = 255; // store second value for refresh

void setup() {
 
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.display();

  PanelStrip.Begin();
  PanelStrip.ClearTo(RgbwColor(0,0,0,255));
  PanelStrip.Show();
  OtherStrip.Begin();
  OtherStrip.ClearTo(RgbwColor(0,0,0,255));
  OtherStrip.Show();

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    display.clear();
    display.display();
    delay ( 300 );
    display.drawString(0, 0, "Connecting\n to\n Wifi ...");
    display.display();
    delay ( 300 );
  }

  display.clear();
  display.drawString(0, 0, "Wifi Connected");
  display.display();

  timeClient.begin();
}

uint8_t ff=0;

void loop() {

  if(timeClient.getSeconds() != oldt) {
    timeClient.update();

    display.clear();
    display.drawString(0, 0, timeClient.getFormattedTime());
    display.display();
    oldt = timeClient.getSeconds();

    if(ff == 0) {
      PanelStrip.SetPixelColor(0, RgbwColor(255,0,0,0));
      PanelStrip.SetPixelColor(1, RgbwColor(0,255,0,0));
      PanelStrip.SetPixelColor(2, RgbwColor(0,0,255,0));
      PanelStrip.SetPixelColor(3, RgbwColor(255,0,0,255));
      PanelStrip.SetPixelColor(4, RgbwColor(0,255,0,255));
      PanelStrip.SetPixelColor(5, RgbwColor(0,0,255,255));
      PanelStrip.SetPixelColor(6, RgbwColor(255,255,255,255));
      PanelStrip.Show();
      ff=1;
    }
    else {
      PanelStrip.SetPixelColor(0, RgbwColor(0,0,0,0));
      PanelStrip.SetPixelColor(1, RgbwColor(0,0,0,0));
      PanelStrip.SetPixelColor(2, RgbwColor(0,0,0,0));
      PanelStrip.SetPixelColor(3, RgbwColor(0,0,0,0));
      PanelStrip.SetPixelColor(4, RgbwColor(0,0,0,0));
      PanelStrip.SetPixelColor(5, RgbwColor(0,0,0,0));
      PanelStrip.SetPixelColor(6, RgbwColor(0,0,0,0));
      PanelStrip.Show();
      ff=0;
    }
  }
}