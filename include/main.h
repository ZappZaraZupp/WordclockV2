#ifndef H_MAIN
#define H_MAIN

#include <Arduino.h>
#include <wire.h>
#include "SSD1306.h"
#include <wc_font.h>

#include <NeoPixelBus.h>

#include <NTP.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFipwd.h>

/////////////////////////////////////
// The display
SSD1306  display(0x3c, 21, 22);

/////////////////////////////////////
// in WiFipwd.h
//const char *ssid     = "ssid";
//const char *password = "password";
////
WiFiUDP ntpUDP;
NTP ntp(ntpUDP);

/////////////////////////////////////
// Char LEDs (Matrix)
const uint8_t PanelWidth = 11;
const uint8_t PanelHeight = 10;
const uint16_t PanelPixelCount = PanelWidth * PanelHeight;
const uint8_t PanelPixelPin = 18;
NeoTopology <RowMajorLayout> topo(PanelWidth, PanelHeight);
NeoPixelBus <NeoGrbwFeature, Neo800KbpsMethod> PanelStrip(PanelPixelCount, PanelPixelPin);

/////////////////////////////////////
// Minutes LEDs (minutes 1-4, 8 touch keys)
const uint16_t MinutesPixelCount = 4;
const uint8_t MinutesPixelPin = 19;
NeoPixelBus <NeoGrbwFeature, Neo800KbpsMethod> MinutesStrip(MinutesPixelCount, MinutesPixelPin);

/////////////////////////////////////
// brightness
const uint8_t LDRPin = A0;
uint8_t minBrightness = 10;
uint8_t maxBrightness = 255;
uint8_t currentBright = 0;

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
// set all Pixels
void setPixels(void);

/////////////////////////////////////
// set Pixels for PanelStrip from panelMask
void setPanelPixel(void);

/////////////////////////////////////
// set Pixels for MinutesStrip from minutesMask
void setMinutesPixel(void);

/////////////////////////////////////
// hours text
void HourText(uint8_t h);

/////////////////////////////////////
// get time and set panelMask
void getTimeText(void);

/////////////////////////////////////
// get Minutes and set minutesMask
void getMinutesText(void);

/////////////////////////////////////
// RGB Collor wheel
RgbwColor colorWheel(uint16_t wheelsteps, uint16_t curstep);

/////////////////////////////////////
// Tests
void PixelColorWheel(NeoPixelBus <NeoGrbwFeature, Neo800KbpsMethod> *strip, uint8_t wait);

#endif