// 10*16-bit "mask" for each pixel (one variable is to small for 110 pixels)
uint16_t mask[10];

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

// define masks for each word.
#define ES              mask[0] |= 0b1100000000000000
#define IST_1           mask[0] |= 0b0001110000000000
#define FUNF_1          mask[0] |= 0b0000000111100000
#define ZEHN_1          mask[1] |= 0b1111000000000000
#define ZWANZIG         mask[1] |= 0b0000111111100000
#define DREI_1          mask[2] |= 0b1111000000000000
#define VIER_1          mask[2] |= 0b0000111100000000
#define DREIVIERTEL     mask[2] |= 0b1111111111100000
#define VIERTEL         mask[2] |= 0b0000111111100000
#define VOR             mask[3] |= 0b1110000000000000
#define UM              mask[3] |= 0b0000011000000000
#define NACH_1          mask[3] |= 0b0000000111100000
#define HALB            mask[4] |= 0b1111000000000000
#define ELF             mask[4] |= 0b0000011100000000
#define FUNF_2          mask[4] |= 0b0000000111100000
#define EIN             mask[5] |= 0b1110000000000000
#define EINS            mask[5] |= 0b1111000000000000
#define TAG             mask[5] |= 0b0000111000000000
#define ZWEI            mask[5] |= 0b0000000111100000
#define DREI_2          mask[6] |= 0b1111000000000000
#define IST_2           mask[6] |= 0b0001110000000000
#define ZWOLF           mask[6] |= 0b0000001111100000
#define SIEBEN          mask[7] |= 0b1111110000000000
#define VIER_2          mask[7] |= 0b0000000111100000
#define NACH_2          mask[8] |= 0b1111000000000000
#define NACHT           mask[8] |= 0b1111100000000000
#define ACHT            mask[8] |= 0b0111100000000000
#define SECHS           mask[8] |= 0b0000001111100000
#define ZEHN_2          mask[9] |= 0b1111000000000000
#define NEUN            mask[9] |= 0b0001111000000000
#define UHR             mask[9] |= 0b0000000011100000

/*
/////////////////////////////////////
// set matrix
void setPanel() {
  for (int i = 0; i <= 9; i++) { // row
    for (int j = 0; j <= 15; j++) { // column
      x = bitRead(mask[i], 15 - j);
      if (x == 1) {
        PanelStrip.SetPixelColor(topo(j,i),RgbwColor(255,0,0,0))
      }
      else {
        PanelStrip.SetPixelColor(topo(j,i),RgbwColor(0,0,0,0))
      }
    }
  }
}*/