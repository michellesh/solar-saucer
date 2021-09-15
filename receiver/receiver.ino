#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#include <FastLED.h>

#define BRIGHTNESS      255
#define COLOR_ORDER     GRB
#define LED_TYPE        WS2813

#define DATA_PIN_0  0
#define DATA_PIN_1  1
#define DATA_PIN_2  2
#define DATA_PIN_3  3
#define DATA_PIN_4  4
#define DATA_PIN_5  5
#define DATA_PIN_6  6
#define DATA_PIN_7  7
#define DATA_PIN_8  8

#define GPIO_0   0   // D3 data (yellow wire on dots)
#define GPIO_2   2   // D4 clock (green wire on dots)
#define GPIO_13  13  // D7 data (yellow wire on dots)
#define GPIO_15  15  // D8 clock (green wire on dots)

#define LEDS_12  50  // Strip 13
#define LEDS_13  35
#define LEDS_14  35
#define LEDS_15  35
#define LEDS_16  35
#define LEDS_17  49

CRGB leds_0[50];  // Strip 1
CRGB leds_1[50];
CRGB leds_2[50];
CRGB leds_3[50];
CRGB leds_4[50];
CRGB leds_5[50];
CRGB leds_6[50];
CRGB leds_7[50];
CRGB leds_8[50];
CRGB leds_9[50];
CRGB leds_10[50];
CRGB leds_11[50];
CRGB leds_12[LEDS_12];
CRGB leds_13[LEDS_13];
CRGB leds_14[LEDS_14];
CRGB leds_15[LEDS_15];
CRGB leds_16[LEDS_16];
CRGB leds_17[LEDS_17];
CRGB *leds[] = {
  leds_0, leds_1, leds_2, leds_3, leds_4, leds_5, leds_6,
  leds_7, leds_8, leds_9, leds_10, leds_11, leds_12, leds_13,
  leds_14, leds_15, leds_16, leds_17
};

Adafruit_WS2801 dotsInner = Adafruit_WS2801(60, GPIO_0, GPIO_2); // dataPin, clockPin
Adafruit_WS2801 dotsOuter = Adafruit_WS2801(120, GPIO_13, GPIO_15); // dataPin, clockPin

uint8_t boardNumber = 2;

void setup() {
  Serial.begin(115200);
  delay(500);

  //uint8_t boardNumber = WiFi.macAddress() == macAddress1   ? 1
  //                      : WiFi.macAddress() == macAddress2 ? 2
  //                                                         : 3;

  if (boardNumber == 1) {  // Blue group
    FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds[0], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds[1], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds[16], LEDS_16)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds[17], LEDS_17)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);

    dotsInner.begin();
    dotsOuter.begin();

    // Update LED contents, to start they are all 'off'
    dotsInner.show();
    dotsOuter.show();

  } else if (boardNumber == 2) {  // Green group
    FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds[2], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds[3], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_3, COLOR_ORDER>(leds[4], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_4, COLOR_ORDER>(leds[5], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds[6], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds[7], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_7, COLOR_ORDER>(leds[8], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_8, COLOR_ORDER>(leds[9], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);

  } else if (boardNumber == 3) {  // Red group
    FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds[10], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds[11], 50)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_3, COLOR_ORDER>(leds[12], LEDS_12)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_4, COLOR_ORDER>(leds[13], LEDS_13)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds[14], LEDS_14)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
    FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds[15], LEDS_15)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);
  } else {
    Serial.println("MAC address not found");
  }

  FastLED.setBrightness(BRIGHTNESS);
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b) {
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

uint8_t getNumPixels(uint8_t strandNumber) {
  return strandNumber == 12 ? LEDS_12
         : strandNumber == 13 ? LEDS_13
         : strandNumber == 14 ? LEDS_14
         : strandNumber == 15 ? LEDS_15
         : strandNumber == 16 ? LEDS_16
         : strandNumber == 17 ? LEDS_17
         : 50;
}

void loop() {

  if (boardNumber == 1) {  // BLUE
    uint8_t blueStrands[] = {0, 1, 16, 17};
    for(uint8_t i = 0; i < 4; i++) {
      for(uint8_t pixel = 0; pixel < getNumPixels(blueStrands[i]); pixel++) {
        leds[blueStrands[i]][pixel] = CRGB::White;
      }
    }
    for(uint8_t i = 0; i < dotsInner.numPixels(); i++) {
      dotsInner.setPixelColor(i, Color(0, 255, 0));
      dotsInner.show();
    }
    for(uint8_t i = 0; i < dotsOuter.numPixels(); i++) {
      dotsOuter.setPixelColor(i, Color(0, 0, 255));
      dotsOuter.show();
    }

  } else if (boardNumber == 2) {  // GREEN
    for(uint8_t strand = 2; strand < 10; strand++) {
      for(uint8_t pixel = 0; pixel < getNumPixels(strand); pixel++) {
        leds[strand][pixel] = CRGB::White;
      }
    }

  } else {  // RED
    for(uint8_t strand = 10; strand < 16; strand++) {
      for(uint8_t pixel = 0; pixel < getNumPixels(strand); pixel++) {
        leds[strand][pixel] = CRGB::White;
      }
    }
  }

  FastLED.show();
}
