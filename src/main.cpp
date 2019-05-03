#include "main.h"

void setup() {

	Serial.begin(115200);

	LOG("start");
  	LOG(__DATE__);
  	LOG(__TIME__);

	display.init();
	display.setFont(Monospaced_bold_10);
	display.flipScreenVertically();
	display.clear();
	display.display();

	pinMode(Sw1Pin, INPUT);
	pinMode(Sw2Pin, INPUT);
	pinMode(Sw3Pin, INPUT);
	pinMode(Sw4Pin, INPUT);
	pinMode(Sw5Pin, INPUT);
	pinMode(Sw6Pin, INPUT);
	pinMode(Sw7Pin, INPUT);
	pinMode(Sw8Pin, INPUT);
	pinMode(LDRPin, INPUT);
	pinMode(PanelPixelPin, OUTPUT);

	PanelStrip.Begin();
	
	//Test
	display.clear();
	display.drawString(0, 0, "Char Test ...");
	display.display();
	PixelColorWheel(0, PanelPixelCount, 100);
	delay(1000);
	display.clear();
	display.drawString(0, 0, "Minutes Test ...");
	display.display();
	PixelColorWheel(PanelPixelCount, PanelPixelCount + MinutesPixelCount, 100);
	delay(1000);
	PanelStrip.ClearTo(0);
	PanelStrip.Show();
	//EndTest

	WiFi.begin(ssid, password);

	display.clear();
	int breakout=20;
	while (WiFi.status() != WL_CONNECTED) {
		display.drawString(0, 0, "Connecting\n to\n Wifi ...");
		display.drawString(80, 0, String(breakout));
		display.display();
		delay(500);
		display.clear();
		display.display();
		delay(500);
		if((breakout-=1)==0) {break;}
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

	if (ntp.seconds() != oldt) {
		currentBright = minBrightness+ (maxBrightness - minBrightness) * (4096.0 - analogRead(A0)) / 4096.0;
		ntp.update();

		display.clear();
		//display.setFont(Monospaced_bold_28);
		display.setFont(Monospaced_bold_16);
		display.drawString(0, 0, ntp.formattedTime("%T"));
		display.drawString(0, 20, ntp.formattedTime("%F"));

		display.setFont(Monospaced_bold_10);
		display.drawString(0, 40, String(currentBright));
		if (digitalRead(Sw1Pin)) {
			display.drawString(0, 50, "1");
		}
		if (digitalRead(Sw2Pin)) {
			display.drawString(10, 50, "2");
		}
		if (digitalRead(Sw3Pin)) {
			display.drawString(20, 50, "3");
		}
		if (digitalRead(Sw4Pin)) {
			display.drawString(30, 50, "4");
		}
		if (digitalRead(Sw5Pin)) {
			display.drawString(40, 50, "5");
		}
		if (digitalRead(Sw6Pin)) {
			display.drawString(50, 50, "6");
		}
		if (digitalRead(Sw7Pin)) {
			display.drawString(60, 50, "7");
		}
		if (digitalRead(Sw8Pin)) {
			display.drawString(70, 50, "8");
		}
		display.display();

		setPixels();
		oldt = ntp.seconds();
	}
	if(PanelAnimation.IsAnimating()) {
		PanelAnimation.UpdateAnimations();
		PanelStrip.Show();
	}
	else {
		SetupMinutesAnimation();
		SetupPanelAnimation();
	}
}

/////////////////////////////////////
// set all Pixels
void setPixels() {
	getTimeText();
	getMinutesText();
}

/////////////////////////////////////
// 
void SetupPanelAnimation() { // initialize AnimationState from getPixelColor and panelMask
	uint8_t x = 0;
	for (int i = 0; i < PanelHeight; i++) { // row
		//LOG(String(i)+" : "+String(panelMask[i]));
		for (int j = 0; j < PanelWidth; j++) { // column
			x = bitRead(panelMask[i], 15 - j);
			if (x == 1) {
				StripState[topo.Map(j, i)].StartingColor = PanelStrip.GetPixelColor(topo.Map(j, i));
				StripState[topo.Map(j, i)].EndingColor = RgbwColor(0*currentBright, 0*currentBright, 0*currentBright, 255*currentBright);
			}
			else {
				StripState[topo.Map(j, i)].StartingColor = PanelStrip.GetPixelColor(topo.Map(j, i));
				StripState[topo.Map(j, i)].EndingColor = RgbwColor(0, 0, 0, 0);
			}
			// Start anim only, if pixel has changed
			if(StripState[topo.Map(j, i)].StartingColor != StripState[topo.Map(j, i)].EndingColor) {
				PanelAnimation.StartAnimation(topo.Map(j, i),50,FadeAnim);
			}
		}
	}
}

void SetupMinutesAnimation() { // initialize AnimationState from getPixelColor and minutesMask
	uint8_t x = 0;
	for (uint8_t j = 0; j <= 3; j++) {
		x = bitRead(minutesMask, 7-j);
		if (x == 1) {
            StripState[MinutesPixelStart+j].StartingColor = PanelStrip.GetPixelColor(MinutesPixelStart+j);
			StripState[MinutesPixelStart+j].EndingColor = RgbwColor(0*currentBright, 255*currentBright, 0*currentBright, 0*currentBright);
		}
		else {
            StripState[MinutesPixelStart+j].StartingColor = PanelStrip.GetPixelColor(MinutesPixelStart+j);
			StripState[MinutesPixelStart+j].EndingColor = RgbwColor(0, 0, 0, 0);
		}
		// Start anim only, if pixel has changed
        if(StripState[j].StartingColor != StripState[j].EndingColor) {
            PanelAnimation.StartAnimation(MinutesPixelStart+j,50,FadeAnim);
        }
	}
}

void FadeAnim(AnimationParam param) {
    //simple linear blend
    RgbwColor updatedColor = RgbwColor::LinearBlend(
        StripState[param.index].StartingColor,
        StripState[param.index].EndingColor,
        param.progress);
    // apply the color to the strip
    PanelStrip.SetPixelColor(param.index, updatedColor);
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
		// special cases
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
		HourText(ntp.hours() == 1 ? 100 : ntp.hours()); // "Es ist Ein Uhr" (nicht: "Es ist Eins Uhr" ;-)
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
	minutesMask = 0;
	switch (ntp.minutes() % 5) {
	case 1:
		minutesMask = 0b10000000;
		//minutesMask = 0b10000000;
		break;
	case 2:
		minutesMask = 0b11000000;
		//minutesMask = 0b01000000;
		break;
	case 3:
		minutesMask = 0b11100000;
		//minutesMask = 0b00100000;
		break;
	case 4:
		minutesMask = 0b11110000;
		//minutesMask = 0b00010000;
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
void PixelColorWheel(uint8_t from, uint8_t to, uint8_t wait) {
	// todo: check borders
	for (uint8_t i = from;  i < to; i++) {
		PanelStrip.SetPixelColor(i, colorWheel(to-from, i-from));
		PanelStrip.Show();
		delay(wait);
	}
}
