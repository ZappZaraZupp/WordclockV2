#include <Arduino.h>
#include <wire.h>
#include "SSD1306.h"
#include <wc_font.h>

#include <NeoPixelBus.h>

#include <NTP.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFipwd.h>

#include <wordclock.h>

// The display
SSD1306  display(0x3c, 21, 22);

////
// in WiFipwd.h
//const char *ssid     = "ssid";
//const char *password = "password";
////
WiFiUDP ntpUDP;
NTP ntp(ntpUDP);

// Char LEDs (Matrix)
const uint8_t PanelWidth = 11;
const uint8_t PanelHeight = 10;
const uint16_t PanelPixelCount = PanelWidth * PanelHeight;
const uint8_t PanelPixelPin = 18;
NeoTopology <RowMajorLayout> topo(PanelWidth, PanelHeight);
NeoPixelBus <NeoGrbwFeature, Neo800KbpsMethod> PanelStrip(PanelPixelCount, PanelPixelPin);

// Other LEDs (minutes 1-4, 8 touch keys)
const uint16_t OtherPixelCount = 12;
const uint8_t OtherPixelPin = 19;
NeoPixelBus <NeoGrbwFeature, Neo800KbpsMethod> OtherStrip(PanelPixelCount, PanelPixelPin);

// brightness
const uint8_t LDRPin = A0;
uint8_t minBrightness = 10;
uint8_t maxBrightness = 255;

// switches
const uint8_t Sw1Pin = 39;
const uint8_t Sw2Pin = 34;
const uint8_t Sw3Pin = 35;
const uint8_t Sw4Pin = 32;
const uint8_t Sw5Pin = 33;
const uint8_t Sw6Pin = 25;
const uint8_t Sw7Pin = 26;
const uint8_t Sw8Pin = 27;


uint8_t oldt = 255; // store second value for refresh

void setup() {
 
  display.init();
  display.setFont(Monospaced_bold_10);
  display.flipScreenVertically();
  display.clear();
  display.display();

  pinMode(Sw1Pin,INPUT);
  pinMode(Sw2Pin,INPUT);
  pinMode(Sw3Pin,INPUT);
  pinMode(Sw4Pin,INPUT);
  pinMode(Sw5Pin,INPUT);
  pinMode(Sw6Pin,INPUT);
  pinMode(Sw7Pin,INPUT);
  pinMode(Sw8Pin,INPUT);
  pinMode(LDRPin,INPUT);

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

  ntp.ntpServer("192.168.42.99");
  ntp.ruleDST("CEST", Last, Sun, Mar, 2, 120); // last sunday in march 2:00, timetone +120min (+1 GMT + 1h summertime offset)
  ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60); // last sunday in october 3:00, timezone +60min (+1 GMT)
  ntp.begin();


}

uint8_t ff=0;
int bright=0;

void loop() {

  if(ntp.seconds() != oldt) {
    bright = analogRead(A0);
    ntp.update();

    display.clear();
    //display.setFont(Monospaced_bold_28);
    display.setFont(Monospaced_bold_16);
    display.drawString(0, 0, ntp.formattedTime("%T"));
    //display.setFont(Monospaced_bold_16);
    //display.drawString(0, 20, ntp.formattedTime("%F"));
    display.drawString(0, 20, String(bright));
    if(digitalRead(Sw1Pin)) {
      display.drawString(0,40,"1");
    }
    if(digitalRead(Sw2Pin)) {
      display.drawString(10,40,"2");
    }
    if(digitalRead(Sw3Pin)) {
      display.drawString(20,40,"3");
    }
    if(digitalRead(Sw4Pin)) {
      display.drawString(30,40,"4");
    }
    if(digitalRead(Sw5Pin)) {
      display.drawString(40,40,"5");
    }
    if(digitalRead(Sw6Pin)) {
      display.drawString(50,40,"6");
    }
    if(digitalRead(Sw7Pin)) {
      display.drawString(60,40,"7");
    }
    if(digitalRead(Sw8Pin)) {
      display.drawString(70,40,"8");
    }
    display.display();
    oldt = ntp.seconds();

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