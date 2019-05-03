#ifndef H_MAIN
#define H_MAIN

#define LOG(arg) Serial.print("[");Serial.print((const char*)__FUNCTION__);Serial.print("] ");Serial.print(arg);Serial.print("\r\n");
//#define LOG(arg) ;

#include <Arduino.h>

// https://github.com/ThingPulse/esp8266-oled-ssd1306
// ESP8266 and ESP32 Oled Driver for SSD1306 display by Daniel Eichhorn
#include <wire.h>
#include "SSD1306Wire.h"
#include <wc_font.h>

// NeoPixelBus by Michael Miller
// https://github.com/Makuna/NeoPixelBus
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

// NTP by Stefan Staub
// https://github.com/sstaub/NTP
#include <NTP.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFipwd.h>

/////////////////////////////////////
// The display
SSD1306Wire  display(0x3c, 21, 22);

/////////////////////////////////////
// in WiFipwd.h
//const char *ssid     = "ssid";
//const char *password = "password";
////
WiFiUDP ntpUDP;
NTP ntp(ntpUDP);

/////////////////////////////////////
// LEDs
// 1st: the character matrix
// 2nd: the 4 minutes led
const uint8_t PanelWidth = 11;
const uint8_t PanelHeight = 10;
//const uint8_t PanelWidth = 7; //test mit 7 led
//const uint8_t PanelHeight = 1;
const uint8_t PanelPixelCount = PanelWidth * PanelHeight;
const uint8_t PanelPixelPin = 18;
const uint8_t MinutesPixelCount = 4;
const uint8_t MinutesPixelStart = PanelPixelCount;
NeoTopology <RowMajorLayout> topo(PanelWidth, PanelHeight);
NeoPixelBus <NeoGrbwFeature, Neo800KbpsMethod> PanelStrip(PanelPixelCount + MinutesPixelCount, PanelPixelPin);
NeoPixelAnimator PanelAnimation(PanelPixelCount + MinutesPixelCount, NEO_CENTISECONDS);

struct PanelAnimationState
{
    RgbwColor StartingColor;  // the color the animation starts at
    RgbwColor EndingColor; // the color the animation will end at
}; 

PanelAnimationState StripState[PanelPixelCount + MinutesPixelCount];

/////////////////////////////////////
// brightness (value between 0..1)
const uint8_t LDRPin = A0;
float minBrightness = 20.0/4096.0;
float maxBrightness = 1.0;
float currentBright = 0.0;

/////////////////////////////////////
// switches
const uint8_t Sw1Pin = 12;
const uint8_t Sw2Pin = 14;
const uint8_t Sw3Pin = 27;
const uint8_t Sw4Pin = 26;
const uint8_t Sw5Pin = 25;
const uint8_t Sw6Pin = 33;
const uint8_t Sw7Pin = 32;
const uint8_t Sw8Pin = 35;

/////////////////////////////////////
uint8_t oldt = 255; // store second value for refresh

/////////////////////////////////////
// 10*16-bit "panelMask" for each pixel (one variable is to small for 110 pixels)
uint16_t panelMask[10];
uint8_t minutesMask;

// ESKISTEFÜNF
// ZEHNZWANZIG
// DREIVIERTEL
// VORNEUMNACH
// HALBXELFÜNF
// EINSTAGZWEI
// DREISTZWÖLF
// SIEBENZVIER
// NACHTASECHS
// ZEHNEUNKUHR

// define panelMasks for each word.
#define ES              panelMask[0] |= 0b1100000000000000
#define IST_1           panelMask[0] |= 0b0001110000000000
#define FUNF_1          panelMask[0] |= 0b0000000111100000
#define ZEHN_1          panelMask[1] |= 0b1111000000000000
#define ZWANZIG         panelMask[1] |= 0b0000111111100000
#define DREI_1          panelMask[2] |= 0b1111000000000000
#define VIER_1          panelMask[2] |= 0b0000111100000000
#define DREIVIERTEL     panelMask[2] |= 0b1111111111100000
#define VIERTEL         panelMask[2] |= 0b0000111111100000
#define VOR             panelMask[3] |= 0b1110000000000000
#define UM              panelMask[3] |= 0b0000011000000000
#define NACH_1          panelMask[3] |= 0b0000000111100000
#define HALB            panelMask[4] |= 0b1111000000000000
#define ELF             panelMask[4] |= 0b0000011100000000
#define FUNF_2          panelMask[4] |= 0b0000000111100000
#define EIN             panelMask[5] |= 0b1110000000000000
#define EINS            panelMask[5] |= 0b1111000000000000
#define TAG             panelMask[5] |= 0b0000111000000000
#define ZWEI            panelMask[5] |= 0b0000000111100000
#define DREI_2          panelMask[6] |= 0b1111000000000000
#define IST_2           panelMask[6] |= 0b0001110000000000
#define ZWOLF           panelMask[6] |= 0b0000001111100000
#define SIEBEN          panelMask[7] |= 0b1111110000000000
#define VIER_2          panelMask[7] |= 0b0000000111100000
#define NACH_2          panelMask[8] |= 0b1111000000000000
#define NACHT           panelMask[8] |= 0b1111100000000000
#define ACHT            panelMask[8] |= 0b0111100000000000
#define SECHS           panelMask[8] |= 0b0000001111100000
#define ZEHN_2          panelMask[9] |= 0b1111000000000000
#define NEUN            panelMask[9] |= 0b0001111000000000
#define UHR             panelMask[9] |= 0b0000000011100000


/////////////////////////////////////
// set all Masks for current time
void setPixels(void);

/////////////////////////////////////
// get time and set panelMask
void getTimeText(void);

/////////////////////////////////////
// hours text
void HourText(uint8_t h);

/////////////////////////////////////
// setup Pixelsanimation for PanelStrip from panelMask
void SetupPanelAnimation();

/////////////////////////////////////
// get Minutes and set minutesMask
void getMinutesText(void);

/////////////////////////////////////
// set Pixelsanmiation for MinutesStrip from minutesMask
void SetupMinutesAnimation();

/////////////////////////////////////
// simple fading
void FadeAnim(AnimationParam param); 

/////////////////////////////////////
// RGB Collor wheel
RgbwColor colorWheel(uint16_t wheelsteps, uint16_t curstep);

/////////////////////////////////////
// Tests
void PixelColorWheel(uint8_t from, uint8_t to, uint8_t wait);

#endif
