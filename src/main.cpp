#include "main.h"

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

  display.clear();
  display.drawString(0, 0, "Pixel Test ...");
  display.display();
  PanelStrip.Begin();
  PanelStrip.ClearTo(RgbwColor(0,0,0,255));
  PanelStrip.Show();
  MinutesStrip.Begin();
  MinutesStrip.ClearTo(RgbwColor(0,0,0,255));
  MinutesStrip.Show();

  //Test
  delay(1000);
  PixelColorWheel(&PanelStrip,10);
  PixelColorWheel(&MinutesStrip,100);
  //EndTest

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

void loop() {

  if(ntp.seconds() != oldt) {
    currentBright = (int)((4096.0-analogRead(A0))/4096.0*255.0);
    ntp.update();

    display.clear();
    //display.setFont(Monospaced_bold_28);
    display.setFont(Monospaced_bold_16);
    display.drawString(0, 0, ntp.formattedTime("%T"));
    display.drawString(0, 20, ntp.formattedTime("%F"));
    
    display.setFont(Monospaced_bold_10);
    display.drawString(0, 40, String(currentBright));
    if(digitalRead(Sw1Pin)) {
      display.drawString(0,50,"1");
    }
    if(digitalRead(Sw2Pin)) {
      display.drawString(10,50,"2");
    }
    if(digitalRead(Sw3Pin)) {
      display.drawString(20,50,"3");
    }
    if(digitalRead(Sw4Pin)) {
      display.drawString(30,50,"4");
    }
    if(digitalRead(Sw5Pin)) {
      display.drawString(40,50,"5");
    }
    if(digitalRead(Sw6Pin)) {
      display.drawString(50,50,"6");
    }
    if(digitalRead(Sw7Pin)) {
      display.drawString(60,50,"7");
    }
    if(digitalRead(Sw8Pin)) {
      display.drawString(70,50,"8");
    }
    display.display();

    setPixels();
    PanelStrip.Show();
    MinutesStrip.Show();

    oldt = ntp.seconds();
  }
}

/////////////////////////////////////
// set all Pixels
void setPixels() {
    getTimeText();
    getMinutesText();
    setPanelPixel();
    setMinutesPixel();
}

/////////////////////////////////////
// set Pixels for PanelStrip from panelMask
void setPanelPixel() {
  uint8_t x=0;
  for (int i = 0; i <= 9; i++) { // row
    for (int j = 0; j <= 15; j++) { // column
      x = bitRead(panelMask[i], 15 - j);
      if (x == 1) {
        PanelStrip.SetPixelColor(topo.Map(j,i),RgbwColor(255,0,0,0));
      }
      else {
        PanelStrip.SetPixelColor(topo.Map(j,i),RgbwColor(0,0,0,0));
      }
    }
  }
}

/////////////////////////////////////
// set Pixels for OtherStrip from otherMask
void setMinutesPixel() {
  uint8_t x=0;
  for (int j = 0; j <= 3; j++) { 
    x = bitRead(minutesMask, 8 - j);
    if (x == 1) {
      MinutesStrip.SetPixelColor(j,RgbwColor(255,0,0,0));
    }
    else {
      MinutesStrip.SetPixelColor(j,RgbwColor(0,0,0,0));
    }
  }
}

/////////////////////////////////////
// hours text
void HourText(uint8_t h) {
  if (h < 100) { // >=100 special cases
    h %= 12;
  }
  switch (h) {
    case 0:
    case 12:
      ZWOLF;
      break;
    case 1:
      EINS;
      break;
    case 2:
      ZWEI;
      break;
    case 3:
      DREI_2;
      break;
    case 4:
      VIER_2;
      break;
    case 5:
      FUNF_2;
      break;
    case 6:
      SECHS;
      break;
    case 7:
      SIEBEN;
      break;
    case 8:
      ACHT;
      break;
    case 9:
      NEUN;
      break;
    case 10:
      ZEHN_2;
      break;
    case 11:
      ELF;
      break;
    // specail cases
    case 100:
      EIN;
      break;
  }
}

/////////////////////////////////////
// get time and set panelMask
void getTimeText() {
  // panelMask-lines reset
  for (uint8_t i = 0; i < 10; i++) {
    panelMask[i] = 0;
  }

  // ES IST
  ES;
  IST_1;

  if (ntp.minutes() < 5) {
    HourText(ntp.hours() == 1 ? 100 : ntp.hours() ); // "Es ist Ein Uhr" (nicht: "Es ist Eins Uhr" ;-)
    UHR;
  }
  else if (ntp.minutes() < 10) {
    //00:05	Es ist fünf[0] nach xx
    FUNF_1;
    NACH_1;
    HourText(ntp.hours());
  }
  else if (ntp.minutes() < 15) {
    //00:10	Es ist zehn[1] nach xx
    ZEHN_1;
    NACH_1;
    HourText(ntp.hours());
  }
  else if (ntp.minutes() < 20) {
    //00:15	Es ist viertel nach xx
    VIERTEL;
    NACH_1;
    HourText(ntp.hours());
  }
  else if (ntp.minutes() < 25) {
    //00:20	Es ist zwanzig nach xx
    ZWANZIG;
    NACH_1;
    HourText(ntp.hours());
  }
  else if (ntp.minutes() < 30) {
    //00:25	Es ist fünf[0] vor halb xx+1
    FUNF_1;
    VOR;
    HALB;
    HourText(ntp.hours() + 1);
  }
  else if (ntp.minutes() < 35) {
    //00:30	Es ist halb xx+1
    HALB;
    HourText(ntp.hours() + 1);
  }
  else if (ntp.minutes() < 40) {
    //00:35	Es ist fünf[0] nach halb xx+1
    FUNF_1;
    NACH_1;
    HALB;
    HourText(ntp.hours() + 1);
  }
  else if (ntp.minutes() < 45) {
    //00:40	Es ist zwanzig vor xx+1
    ZWANZIG;
    VOR;
    HourText(ntp.hours() + 1);
  }
  else if (ntp.minutes() < 50) {
    //00:45	Es ist viertel vor xx+1
    //		Es ist dreiviertel xx+1
    if ((int)random(2) == 0) {
      VIERTEL;
      VOR;
      HourText(ntp.hours() + 1);
    }
    else {
      DREIVIERTEL;
      HourText(ntp.hours() + 1);
    }
  }
  else if (ntp.minutes() < 55) {
    //00:50	Es ist zehn[1] vor xx+1
    ZEHN_1;
    VOR;
    HourText(ntp.hours() + 1);
  }
  else {
    //00:55	Es ist fünf[0] vor xx+1
    FUNF_1;
    VOR;
    HourText(ntp.hours() + 1);
  }
}

/////////////////////////////////////
// get Minutes
void getMinutesText() {
  minutesMask=0;
  switch(ntp.minutes() % 5) {
    case 1:
      minutesMask = 1;
      break;
    case 2:
      minutesMask = 2;
      break;
    case 3:
      minutesMask = 4;
      break;
    case 4:
      minutesMask = 8;
      break;
  }
}

/////////////////////////////////////
// RGB Collor wheel
RgbwColor colorWheel(uint16_t wheelsteps, uint16_t curstep) {

  float p = wheelsteps / 3.0;
  float s = 255.0 / p; // stepsize

  // 255,0,0 --> 0,255,0
  if (curstep < p) {
    return RgbwColor(255 - curstep * s, curstep * s, 0, 0);
  }
  // 0,255,0 --> 0,0,255
  if (curstep < 2 * p) {
    curstep -= p;
    return RgbwColor(0, 255 - curstep * s, curstep * s, 0);
  }
  // 0,0,255 --> 255,0,0
  curstep -= 2 * p;
  return RgbwColor(curstep * s, 0, 255 - curstep * s, 0);
}

/////////////////////////////////////
// Tests
void PixelColorWheel(NeoPixelBus <NeoGrbwFeature, Neo800KbpsMethod> *strip, uint8_t wait) {
  for (uint16_t i = 0; i < strip->PixelCount(); i++) {
    strip->SetPixelColor(i, colorWheel(strip->PixelCount(), i));
    strip->Show();
    delay(wait);
  }
}